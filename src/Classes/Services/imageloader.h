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

#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QObject>
#include <QStandardPaths>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFileInfo>

class ImageLoader : public QObject
{
    Q_OBJECT

private:
    QString m_Path;
    QNetworkAccessManager* m_manager { new QNetworkAccessManager(this) };

public:
    explicit ImageLoader(QObject *parent = nullptr, QString path = "");

    void loadImage(int id, const QString& originalPath);

signals:
    void imageLoaded(QString loadedPath, int id);

public slots:
    void imageDownloaded(QNetworkReply* reply);

};

#endif // IMAGELOADER_H
