#include "offlineimagecacheservice.h"

void OfflineImageCacheService::loadCache()
{
    QDir directory(m_CachePath);
    QStringList images = directory.entryList(QStringList() << "*.*", QDir::Files);
    foreach(QString filename, images) {
        auto id = filename.toLower().replace(".image", "").toInt();
        m_Images->insert(id, m_Protocol + m_CachePath + "/" + filename);
    }
}

void OfflineImageCacheService::imageLoaded(QString loadedPath, int id)
{
    if (m_Images->contains(id)) {
        auto images = *m_Images;
        images[id] = loadedPath;
    } else {
        m_Images->insert(id, m_Protocol + loadedPath);
    }

    ImageLoader* imageLoader = m_RunningLoading->value(id);
    imageLoader->deleteLater();
    m_RunningLoading->remove(id);
}

OfflineImageCacheService::OfflineImageCacheService(QObject *parent) : QObject(parent),
    m_Images(new QHash<int, QString>()),
    m_RunningLoading(new QHash<int, ImageLoader*>())
{
    m_CachePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/imagecache";
#ifdef Q_OS_WIN
    m_Protocol = "file:///";
#else
    m_Protocol = "file://";
#endif

    loadCache();
}

QString OfflineImageCacheService::getReleasePath(int id, QString posterPath)
{
    if (!m_Images->contains(id)) {
        if (!m_RunningLoading->contains(id)) {
            ImageLoader* imageLoader = new ImageLoader(this, m_CachePath);
            m_RunningLoading->insert(id, imageLoader);
            connect(imageLoader, &ImageLoader::imageLoaded, this, &OfflineImageCacheService::imageLoaded);
            imageLoader->loadImage(id, posterPath);
        }
        return posterPath;
    }
    return m_Images->value(id);
}

