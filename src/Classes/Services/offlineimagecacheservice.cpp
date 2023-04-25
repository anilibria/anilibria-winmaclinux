/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2020 Roman Vladimirov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QHashIterator>
#include "offlineimagecacheservice.h"
#include "globalconstants.h"

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

    m_RunningLoading.remove(id);
}

OfflineImageCacheService::OfflineImageCacheService(QObject *parent) : QObject(parent) {
    if (IsPortable) {
        m_CachePath = QDir::currentPath() + "/imagecache";
    } else {
        m_CachePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/imagecache";
    }

    m_imageLoader = new ImageLoader(this, m_CachePath);
    connect(m_imageLoader, &ImageLoader::imageLoaded, this, &OfflineImageCacheService::imageLoaded);

#ifdef Q_OS_WIN
    m_Protocol = "file:///";
#else
    m_Protocol = "file://";
#endif

    loadCache();
}

void OfflineImageCacheService::invalidateReleasePoster(int id)
{
    if (m_Images->contains(id)) {
        QFile file (m_Images->value(id));
        file.remove();
        m_Images->remove(id);
    }
}

void OfflineImageCacheService::clearPosterCache()
{
    QHashIterator<int, QString> iterator(*m_Images);
    while (iterator.hasNext()) {
        iterator.next();
        auto filename = iterator.value();
        QFile file(filename.replace("file:///", "").replace("file://", ""));
        file.remove();
    }
}

QString OfflineImageCacheService::getReleasePath(int id, QString posterPath)
{
    if (!m_Images->contains(id)) {
        if (!m_RunningLoading.contains(id)) {
            m_imageLoader->loadImage(id, posterPath);
            m_RunningLoading.insert(id);
        }
        return posterPath;
    }
    return m_Images->value(id);
}

