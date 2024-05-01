#include <stdexcept>
#include <clocale>
#include <cstring>
#include <QObject>
#include <QtGlobal>
#include <QOpenGLContext>
#include <QGuiApplication>
#include <QOpenGLFramebufferObject>
#include <QtQuick/QQuickWindow>
#include <QtQuick/QQuickView>
#include <QThread>
#include <QStandardPaths>
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
#include <QQuickOpenGLUtils>
#endif
#include "mpvobject.h"

void on_mpv_events(void *ctx)
{
    Q_UNUSED(ctx);
}

void on_mpv_redraw(void *ctx)
{
    MpvObject::on_update(ctx);
}

static void *get_proc_address_mpv(void *ctx, const char *name)
{
    Q_UNUSED(ctx)

    QOpenGLContext *glctx = QOpenGLContext::currentContext();
    if (!glctx) return nullptr;

    return reinterpret_cast<void *>(glctx->getProcAddress(QByteArray(name)));
}

struct nodeAutoFree {
    mpv_node *ptr;

    nodeAutoFree(mpv_node *a_ptr) : ptr(a_ptr) {}
    ~nodeAutoFree() { mpv_free_node_contents(ptr); }
};

struct ErrorReturn
{
    int error;

    ErrorReturn() : error(0) {}
    explicit ErrorReturn(int err) : error(err) {}
};

struct nodeBuilder {
    nodeBuilder(const QVariant& v) {
        set(&node_, v);
    }
    ~nodeBuilder() {
        free_node(&node_);
    }
    mpv_node *node() { return &node_; }
private:
    Q_DISABLE_COPY(nodeBuilder)
    mpv_node node_;
    mpv_node_list *create_list(mpv_node *dst, bool is_map, int num) {
        dst->format = is_map ? MPV_FORMAT_NODE_MAP : MPV_FORMAT_NODE_ARRAY;
        mpv_node_list *list = new mpv_node_list();
        dst->u.list = list;
        if (!list)
            goto err;
        list->values = new mpv_node[num]();
        if (!list->values)
            goto err;
        if (is_map) {
            list->keys = new char*[num]();
            if (!list->keys)
                goto err;
        }
        return list;
    err:
        free_node(dst);
        return NULL;
    }
    char *dup_qstring(const QString &s) {
        QByteArray b = s.toUtf8();
        char *r = new char[b.size() + 1];
        if (r)
            std::memcpy(r, b.data(), b.size() + 1);
        return r;
    }
    bool test_type(const QVariant &v, QMetaType::Type t) {
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
        return v.typeId() == static_cast<int>(t);
#else
        return static_cast<int>(v.type()) == static_cast<int>(t);
#endif
    }

    void set(mpv_node *dst, const QVariant &src) {
        if (test_type(src, QMetaType::QString)) {
            dst->format = MPV_FORMAT_STRING;
            dst->u.string = dup_qstring(src.toString());
            if (!dst->u.string)
                goto fail;
        } else if (test_type(src, QMetaType::Bool)) {
            dst->format = MPV_FORMAT_FLAG;
            dst->u.flag = src.toBool() ? 1 : 0;
        } else if (test_type(src, QMetaType::Int) ||
                   test_type(src, QMetaType::LongLong) ||
                   test_type(src, QMetaType::UInt) ||
                   test_type(src, QMetaType::ULongLong))
        {
            dst->format = MPV_FORMAT_INT64;
            dst->u.int64 = src.toLongLong();
        } else if (test_type(src, QMetaType::Double)) {
            dst->format = MPV_FORMAT_DOUBLE;
            dst->u.double_ = src.toDouble();
        } else if (src.canConvert<QVariantList>()) {
            QVariantList qlist = src.toList();
            mpv_node_list *list = create_list(dst, false, qlist.size());
            if (!list)
                goto fail;
            list->num = qlist.size();
            for (int n = 0; n < qlist.size(); n++)
                set(&list->values[n], qlist[n]);
        } else if (src.canConvert<QVariantMap>()) {
            QVariantMap qmap = src.toMap();
            mpv_node_list *list = create_list(dst, true, qmap.size());
            if (!list)
                goto fail;
            list->num = qmap.size();
            for (int n = 0; n < qmap.size(); n++) {
                auto keys = qmap.keys();
                auto key = keys[n];
                list->keys[n] = dup_qstring(key);
                if (!list->keys[n]) {
                    free_node(dst);
                    goto fail;
                }
                auto values = qmap.values();
                auto value = values[n];
                set(&list->values[n], value);
            }
        } else {
            goto fail;
        }
        return;
    fail:
        dst->format = MPV_FORMAT_NONE;
    }
    void free_node(mpv_node *dst) {
        switch (dst->format) {
        case MPV_FORMAT_STRING:
            delete[] dst->u.string;
            break;
        case MPV_FORMAT_NODE_ARRAY:
        case MPV_FORMAT_NODE_MAP: {
            mpv_node_list *list = dst->u.list;
            if (list) {
                for (int n = 0; n < list->num; n++) {
                    if (list->keys)
                        delete[] list->keys[n];
                    if (list->values)
                        free_node(&list->values[n]);
                }
                delete[] list->keys;
                delete[] list->values;
            }
            delete list;
            break;
        }
        default: ;
        }
        dst->format = MPV_FORMAT_NONE;
    }
};

class MpvRenderer : public QQuickFramebufferObject::Renderer
{
    MpvObject *obj;

public:
    MpvRenderer(MpvObject *new_obj)
        : obj{new_obj}
    {
        mpv_set_wakeup_callback(obj->mpv, on_mpv_events, obj);
    }

    virtual ~MpvRenderer()
    {}

    // This function is called when a new FBO is needed.
    // This happens on the initial frame.
    QOpenGLFramebufferObject * createFramebufferObject(const QSize &size)
    {
        if (!obj->mpv_gl)
        {
            mpv_opengl_init_params gl_init_params{get_proc_address_mpv, nullptr};
            mpv_render_param params[]{
                {MPV_RENDER_PARAM_API_TYPE, const_cast<char *>(MPV_RENDER_API_TYPE_OPENGL)},
                {MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &gl_init_params},
                {MPV_RENDER_PARAM_INVALID, nullptr}
            };
            if (mpv_render_context_create(&obj->mpv_gl, obj->mpv, params) < 0) {
                throw std::runtime_error("failed to initialize mpv GL context");
            }
            mpv_render_context_set_update_callback(obj->mpv_gl, on_mpv_redraw, obj);
        }

        return QQuickFramebufferObject::Renderer::createFramebufferObject(size);
    }

    void render()
    {
        #if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
            QQuickOpenGLUtils::resetOpenGLState();
        #else
            obj->window()->resetOpenGLState();
        #endif

        QOpenGLFramebufferObject *fbo = framebufferObject();
            mpv_opengl_fbo mpfbo{.fbo = static_cast<int>(fbo->handle()), .w = fbo->width(), .h = fbo->height(), .internal_format = 0};
            int flip_y{0};

            mpv_render_param params[] = {
                {MPV_RENDER_PARAM_OPENGL_FBO, &mpfbo},
                {MPV_RENDER_PARAM_FLIP_Y, &flip_y},
                {MPV_RENDER_PARAM_INVALID, nullptr}
            };
            mpv_render_context_render(obj->mpv_gl, params);

#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
            QQuickOpenGLUtils::resetOpenGLState();
#else
            obj->window()->resetOpenGLState();
        #endif
    }
};

MpvObject::MpvObject(QQuickItem * parent)
    : QQuickFramebufferObject(parent)
{
    setlocale(LC_NUMERIC, "C"); //fix for linux/macos
    mpv = mpv_create();
    if (!mpv) throw std::runtime_error("could not create mpv context");

    mpv_set_option_string(mpv, "terminal", "yes");
    mpv_set_option_string(mpv, "msg-level", "all=v");
    mpv_set_option_string(mpv, "cache", "yes");
    mpv_set_option_string(mpv, "cache-secs", "20");
    mpv_set_option_string(mpv, "network-timeout", "20");
    mpv_set_option_string(mpv, "framedrop", "decoder");
    mpv_set_option_string(mpv, "demuxer-termination-timeout", "5");
    mpv_set_option_string(mpv, "demuxer-cache-wait", "yes");
    mpv_set_option_string(mpv, "hwdec", "auto-safe");
    mpv_set_option_string(mpv, "config", "yes");
    auto mpvLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/mpv";
    qDebug() << "MPV config location: " << mpvLocation;
    QByteArray byteArray = mpvLocation.toLocal8Bit();
    auto pathToCache = byteArray.data();
    mpv_set_option_string(mpv, "config-dir", pathToCache);

    if (mpv_initialize(mpv) < 0) throw std::runtime_error("could not initialize mpv context");

    mpv_set_option_string(mpv, "vo", "opengl-cb");

    mpv_observe_property(mpv, 0, "duration", MPV_FORMAT_DOUBLE);
    mpv_observe_property(mpv, 0, "time-pos", MPV_FORMAT_DOUBLE);
    mpv_observe_property(mpv, 0, "mute", MPV_FORMAT_FLAG);
    mpv_observe_property(mpv, 0, "pause", MPV_FORMAT_FLAG);
    mpv_observe_property(mpv, 0, "paused-for-cache", MPV_FORMAT_FLAG);

    //crop=1280x720+0+0

    m_checkTimer = startTimer(100);

    connect(this, &MpvObject::onUpdate, this, &MpvObject::doUpdate, Qt::QueuedConnection);
}

MpvObject::~MpvObject()
{
    if (mpv_gl) mpv_render_context_free(mpv_gl);

    mpv_terminate_destroy(mpv);
}

void MpvObject::on_update(void *ctx)
{
    MpvObject *self = (MpvObject *)ctx;
    emit self->onUpdate();
}

void MpvObject::doUpdate()
{
    update();
}

void MpvObject::command(const QVariant& params)
{
    makeMpvCommand(params);
}

void MpvObject::setProperty(const QString& name, const QVariant& value)
{
    setMpvProperty(name, value);
}

QQuickFramebufferObject::Renderer *MpvObject::createRenderer() const
{
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    window()->setPersistentGraphics(true);
#else
    window()->setPersistentOpenGLContext(true);
#endif

    window()->setPersistentSceneGraph(true);
    return new MpvRenderer(const_cast<MpvObject *>(this));
}

int MpvObject::volume() noexcept
{
    auto value = getMpvProperty("volume");
    if (value.userType() == QMetaType::Int) value = value.toInt();

    return 100;
}

void MpvObject::setVolume(int volume) noexcept
{
    setMpvProperty("volume", volume);

    emit volumeChanged();
}

bool MpvObject::muted() noexcept
{
    auto value = getMpvProperty("mute");
    if (value.userType() == QMetaType::Bool) return value.toBool();

    return false;
}

void MpvObject::setMuted(bool muted) noexcept
{
    setMpvProperty("mute", muted);

    emit mutedChanged();
}

void MpvObject::setSource(const QString& source) noexcept
{
    if (source == m_source) return;

    m_source = source;

    QStringList items;
    items.append("loadfile");
    items.append(source);
    makeMpvCommand(QVariant(items));

    emit sourceChanged();
}

float MpvObject::playbackRate() noexcept
{
    auto value = getMpvProperty("speed");
    if (value.userType() == QMetaType::Double) return value.toFloat();

    return 1.0;
}

void MpvObject::setPlaybackRate(float playbackRate) noexcept
{
    setMpvProperty("speed", (double)playbackRate);

    emit playbackRateChanged();
}

void MpvObject::play()
{
    setMpvProperty("pause", false);
    emit playbackChanged(playedPlayback);
}

void MpvObject::pause()
{
    setMpvProperty("pause", true);
    emit playbackChanged(pausedPlayback);
}

void MpvObject::stop()
{
    setMpvProperty("pause", true);
    emit playbackChanged(stopedPlayback);
}

void MpvObject::seek(int position) noexcept
{
    QStringList items;
    items.append("seek");
    items.append(QString::number(position / 1000));
    items.append("absolute");
    makeMpvCommand(QVariant(items));
}

void MpvObject::setCropMode() noexcept
{
    auto elementWidth = width();

    auto originalHeight = getMpvProperty("height").toInt();
    auto originalWidth = getMpvProperty("width").toInt();

    if (elementWidth >= originalWidth) {
        //don't make sense
        mpv_set_option_string(mpv, "video-crop", "");
        return;
    }

    auto widthDifference = (originalWidth - elementWidth) / 2;
    auto newWidth = originalWidth - widthDifference;
    auto newHeight = originalHeight;

    auto value = QString::number(newWidth) + "x" + QString::number(newHeight) + "+" + QString::number(widthDifference) + "+0";
    mpv_set_option_string(mpv, "video-crop", value.toStdString().c_str());
}

void MpvObject::revertCropMode() noexcept
{
    mpv_set_option_string(mpv, "video-crop", "");
}

void MpvObject::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    auto playerEvent = mpv_wait_event(mpv, 0);
    if (playerEvent == nullptr) return;

    if (playerEvent->event_id == MPV_EVENT_START_FILE) {
        qDebug() << "File Started!!!!";
    }
    if(playerEvent->event_id == MPV_EVENT_FILE_LOADED) {
        emit fileLoaded();
        qDebug() << "File Loaded!!!!";
    }
    if(playerEvent->event_id == MPV_EVENT_COMMAND_REPLY) {
        qDebug() << "Command reply!!!!";
    }
    if (playerEvent->event_id == MPV_EVENT_IDLE) {
        if (m_duration > 0 && m_position > 0) {
            qDebug() << "End reached file!!!!";
            emit endFileReached();
        }
    }
    if(playerEvent->event_id == MPV_EVENT_PROPERTY_CHANGE) {
        auto eventData = static_cast<mpv_event_property*>(playerEvent->data);
        if (eventData == nullptr) return;
        if (eventData->format == MPV_FORMAT_NONE) return;

        auto propertyName = QString(eventData->name);

        if (propertyName == "duration") {
            auto duration = getDoubleFromEventData(eventData->data);
            m_duration = duration * 1000;
            emit durationChanged();
        }
        if (propertyName == "time-pos") {
            auto position = getDoubleFromEventData(eventData->data);
            auto newPosition = position * 1000;
            if (newPosition != m_position) {
                m_position = newPosition;
                if (m_duration > 0) emit positionChanged();
            }
        }

        if (propertyName == "paused-for-cache") {
            auto isBuffering = getBoolFromEventData(eventData->data);
            if (isBuffering) {
                emit startBuffering();
            } else {
                emit endBuffered();
            }
        }

        //seeking ???

        if (propertyName == "pause") {
            m_paused = getBoolFromEventData(eventData->data);
            emit playbackChanged(m_paused ? pausedPlayback : playedPlayback);
        }
    }
    //MPV_EVENT_GET_PROPERTY_REPLY, MPV_EVENT_SET_PROPERTY_REPLY

}

QString MpvObject::getStringFromEventData(void *data) const noexcept
{
    return QString::fromUtf8(static_cast<char*>(data));
}

qlonglong MpvObject::getInt64FromEventData(void *data) const noexcept
{
    auto value = static_cast<qlonglong*>(data);
    return *value;
}

double MpvObject::getDoubleFromEventData(void *data) const noexcept
{
    auto value = static_cast<double*>(data);
    return *value;
}

bool MpvObject::getBoolFromEventData(void *data) const noexcept
{
    auto value = *static_cast<int*>(data);
    return value == 1 ? true : false;
}

int MpvObject::setMpvProperty(const QString &name, const QVariant &v)
{
    nodeBuilder node(v);
    return mpv_set_property(mpv, name.toUtf8().data(), MPV_FORMAT_NODE, node.node());
}

QVariant MpvObject::getMpvProperty(const QString &name)
{
    mpv_node node;
    int err = mpv_get_property(mpv, name.toUtf8().data(), MPV_FORMAT_NODE, &node);
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    if (err < 0) return QVariant::fromValue(ErrorReturn(err));
#else
    if (err < 0) return QVariant::fromValue(err);
#endif
    nodeAutoFree f(&node);
    return nodeToVariant(&node);
}

QVariant MpvObject::nodeToVariant(const mpv_node *node)
{
    switch (node->format) {
        case MPV_FORMAT_STRING:
            return QVariant(QString::fromUtf8(node->u.string));
        case MPV_FORMAT_FLAG:
            return QVariant(static_cast<bool>(node->u.flag));
        case MPV_FORMAT_INT64:
            return QVariant(static_cast<qlonglong>(node->u.int64));
        case MPV_FORMAT_DOUBLE:
            return QVariant(node->u.double_);
        case MPV_FORMAT_NODE_ARRAY: {
            mpv_node_list *list = node->u.list;
            QVariantList qlist;
            for (int n = 0; n < list->num; n++) {
                qlist.append(nodeToVariant(&list->values[n]));
            }
            return QVariant(qlist);
        }
        case MPV_FORMAT_NODE_MAP: {
            mpv_node_list *list = node->u.list;
            QVariantMap qmap;
            for (int n = 0; n < list->num; n++) {
                qmap.insert(QString::fromUtf8(list->keys[n]), nodeToVariant(&list->values[n]));
            }
            return QVariant(qmap);
        }
        default: return QVariant();
        }
}

QVariant MpvObject::makeMpvCommand(const QVariant &params)
{
    nodeBuilder node(params);
    mpv_node res;
    int err = mpv_command_node(mpv, node.node(), &res);
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    if (err < 0) return QVariant::fromValue(ErrorReturn(err));
#else
    if (err < 0) return QVariant::fromValue(err);
#endif
    nodeAutoFree f(&res);
    return nodeToVariant(&res);
}
