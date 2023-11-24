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

ImageLoader::ImageLoader(QObject *parent, QString path) : QObject(parent), m_Path(path)
{
    connect(m_manager,&QNetworkAccessManager::finished,this,&ImageLoader::imageDownloaded);
}

void ImageLoader::loadImage(int id, const QString& originalPath)
{
    auto url = QUrl(originalPath);
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "Anilibria CP Client");

    auto reply = m_manager->get(request);
    reply->setProperty("identifier", id);
}

void ImageLoader::imageDownloaded(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::TimeoutError) return;
    if (reply->error() == QNetworkReply::ProtocolFailure) return;
    if (reply->error() == QNetworkReply::HostNotFoundError) return;

    auto idProperty = reply->property("identifier");
    auto id = idProperty.toInt();

    QVariant statusCode = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
    if ( statusCode.isValid() && statusCode.toInt() != 200) return;

    auto contentTypeHeader = reply->header(QNetworkRequest::ContentTypeHeader);
    if (contentTypeHeader.isValid() && contentTypeHeader.userType() == QMetaType::QString) {
        auto contentType = contentTypeHeader.toString().toLower().trimmed();

        if (!(contentType == "image/jpeg" || contentType == "image/webp" || contentType == "image/png")) return;
    }

    QByteArray data = reply->readAll();

    if (data.length() == 0) return;

    auto fullPath = m_Path + "/" + QString::number(id) + ".image";
    if (QFileInfo::exists(fullPath)) {
        QFile removedFile (fullPath);
        removedFile.remove();
    }

    QFile imageFile(fullPath);
    imageFile.open(QFile::WriteOnly);
    imageFile.write(data);
    imageFile.close();

    emit imageLoaded(fullPath, id);
}
