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

#ifndef OFFLINEIMAGECACHESERVICE_H
#define OFFLINEIMAGECACHESERVICE_H

#include <QObject>
#include <QHash>
#include <QDir>
#include <QStandardPaths>
#include "imageloader.h"

class OfflineImageCacheService : public QObject
{
    Q_OBJECT
private:
    QHash<int, QString>* m_Images;
    QHash<int, ImageLoader*>* m_RunningLoading;
    QString m_CachePath;
    QString m_Protocol;

private:
    void loadCache();
    void imageLoaded(QString loadedPath, int id);

public:
    explicit OfflineImageCacheService(QObject *parent = nullptr);

    void invalidateReleasePoster(int id);

    Q_INVOKABLE QString getReleasePath(int id, QString posterPath);

signals:

};

#endif // OFFLINEIMAGECACHESERVICE_H
