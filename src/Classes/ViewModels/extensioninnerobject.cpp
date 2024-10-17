#include <QDebug>
#include "extensioninnerobject.h"

ExtensionInnerObject::ExtensionInnerObject(QObject *parent)
    : QObject{parent}
{}

void ExtensionInnerObject::setup(QMap<QString, QString> *values) noexcept
{
    m_values = values;
}

void ExtensionInnerObject::setupReleases(const ReleasesViewModel *releases) noexcept
{
    m_releases = const_cast<ReleasesViewModel *>(releases);
}

void ExtensionInnerObject::makeHttpGet(const QString &url, const QList<QString> headers, const QJSValue &callback)
{
    emit makeHttpGetHandler(url, headers, callback);
}

void ExtensionInnerObject::makeHttpPost(const QString &url, const QList<QString> headers, const QString &body, const QJSValue &callback)
{
    emit makeHttpPostHandler(url, headers, body, callback);
}

void ExtensionInnerObject::makeHttpPut(const QString &url, const QList<QString> headers, const QString &body, const QJSValue &callback)
{
    emit makeHttpPutHandler(url, headers, body, callback);
}

void ExtensionInnerObject::makeHttpDelete(const QString &url, const QList<QString> headers, const QJSValue &callback)
{
    emit makeHttpDeleteHandler(url, headers, callback);
}

void ExtensionInnerObject::log(const QString &message)
{
    qDebug() << "[extension]: " << message;
}

void ExtensionInnerObject::saveValue(const QString &key, const QString &value)
{
    m_values->insert(key, value);
}

QString ExtensionInnerObject::readValue(const QString &key)
{
    if (!m_values->contains(key)) return "";

    return m_values->value(key);
}

QVariant ExtensionInnerObject::getRelease(int id)
{
    if (m_releases == nullptr) {
        log("Release not filled yet, you need to use another triggers or menus!");
        return QVariant();
    }

    auto release = m_releases->getReleaseById(id);
    if (release == nullptr) {
        log("Release with id " + QString::number(id) + " not founded!");
        return QVariant();
    }

    QVariantMap releaseObject;
    releaseObject["title"] = release->title();
    releaseObject["code"] = release->code();
    releaseObject["season"] = release->season();
    releaseObject["status"] = release->status();
    releaseObject["year"] = release->year();
    releaseObject["description"] = release->description();
    releaseObject["releaseType"] = release->releaseType();
    releaseObject["id"] = release->id();
    releaseObject["countOnlineVideos"] = release->countOnlineVideos();
    releaseObject["countTorrents"] = release->countTorrents();
    releaseObject["announce"] = release->announce();
    releaseObject["originalName"] = release->originalName();
    releaseObject["rating"] = release->rating();
    releaseObject["timestamp"] = release->timestamp();
    releaseObject["type"] = release->type();
    releaseObject["series"] = release->series();
    releaseObject["genres"] = release->genres();
    releaseObject["voicers"] = release->voicers();

    return releaseObject;
}

QList<int> ExtensionInnerObject::getFavorites()
{
    QList<int> result;
    m_releases->getFavoritesIds(&result);

    return result;
}

QList<int> ExtensionInnerObject::getSeens()
{
    QList<int> result;

    m_releases->getSeenIds(&result);

    return result;
}

int ExtensionInnerObject::getReleasesCount()
{
    return m_releases->countReleases();
}

void ExtensionInnerObject::sendInformationNotification(const QString &message)
{
    emit sendInformationNotificationHandler(message);
}
