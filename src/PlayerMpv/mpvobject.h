#ifndef MPVOBJECT_H
#define MPVOBJECT_H

#include <QObject>
#include <QtQuick/QQuickFramebufferObject>

#include <mpv/client.h>
#include <mpv/render_gl.h>

class MpvRenderer;

class MpvObject : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool muted READ muted WRITE setMuted NOTIFY mutedChanged)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(float playbackRate READ playbackRate WRITE setPlaybackRate NOTIFY playbackRateChanged)
    Q_PROPERTY(int position READ position NOTIFY positionChanged)
    Q_PROPERTY(int duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(bool updateEnabled READ updateEnabled WRITE setUpdateEnabled NOTIFY updateEnabledChanged)

    friend class MpvRenderer;

private:
    mpv_handle *mpv;
    mpv_render_context *mpv_gl;
    QString m_source;
    int m_volume { 100 };
    int m_checkTimer { 0 };
    bool m_paused { false };
    int m_duration { 0 };
    int m_position { 0 };
    bool m_updateEnabled { true };

public:
    static void on_update(void *ctx);

    MpvObject(QQuickItem * parent = 0);
    virtual ~MpvObject();
    virtual Renderer *createRenderer() const override;

    int volume() noexcept;
    void setVolume(int volume) noexcept;

    bool muted() noexcept;
    void setMuted(bool muted) noexcept;

    QString source() const noexcept { return m_source; }
    void setSource(const QString& source) noexcept;

    float playbackRate() noexcept;
    void setPlaybackRate(float playbackRate) noexcept;

    int position() const noexcept { return m_position; }
    int duration() const noexcept { return m_duration; }

    bool updateEnabled() const noexcept { return m_updateEnabled; }
    void setUpdateEnabled(bool updateEnabled) noexcept;

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void seek(int position) noexcept;

    void timerEvent(QTimerEvent *event) override;

    mpv_handle* getContext() const noexcept { return mpv; }

private:
    QString getStringFromEventData(void* data) const noexcept;
    qlonglong getInt64FromEventData(void* data) const noexcept;
    double getDoubleFromEventData(void* data) const noexcept;
    bool getBoolFromEventData(void* data) const noexcept;
    int setMpvProperty(const QString &name, const QVariant &v);
    QVariant getMpvProperty(const QString &name);
    QVariant nodeToVariant(const mpv_node *node);
    QVariant makeMpvCommand(const QVariant& params);

private slots:
    void doUpdate();

public slots:
    void command(const QVariant& params);
    void setProperty(const QString& name, const QVariant& value);    

signals:
    void onUpdate();
    void volumeChanged();
    void mutedChanged();
    void sourceChanged();
    void playbackRateChanged();
    void positionChanged();
    void durationChanged();
    void updateEnabledChanged();

};

#endif // MPVOBJECT_H
