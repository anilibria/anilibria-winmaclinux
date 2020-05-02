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

#include "imageloader.h"

ImageLoader::ImageLoader(QObject *parent, QString path) : QObject(parent), m_Id(0), m_OriginalPath(""), m_Path(path)
{
}

void ImageLoader::loadImage(int id, const QString& originalPath)
{
    m_Id = id;
    m_OriginalPath = originalPath;

    auto networkManager = new QNetworkAccessManager(this);
    auto url = QUrl(originalPath);
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "Anilibria CP Client");

    connect(networkManager,&QNetworkAccessManager::finished,this,&ImageLoader::imageDownloaded);

    networkManager->get(request);
}

void ImageLoader::imageDownloaded(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::TimeoutError) return;
    if (reply->error() == QNetworkReply::ProtocolFailure) return;
    if (reply->error() == QNetworkReply::HostNotFoundError) return;

    QByteArray data = reply->readAll();

    if (data.length() == 0) return;

    auto fullPath = m_Path + "/" + QString::number(m_Id) + ".image";
    if (QFileInfo::exists(fullPath)) {
        QFile removedFile (fullPath);
        removedFile.remove();
    }

    QFile imageFile(fullPath);
    imageFile.open(QFile::WriteOnly);
    imageFile.write(data);
    imageFile.close();

    emit imageLoaded(fullPath, m_Id);
}
