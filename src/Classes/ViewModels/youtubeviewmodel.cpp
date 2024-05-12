/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2021 Roman Vladimirov

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

#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include "youtubeviewmodel.h"

YoutubeViewModel::YoutubeViewModel(QObject *parent) : QAbstractListModel(parent),
    m_apiService(new AnilibriaApiService(this)),
    m_youtubeVideos(new QList<YoutubeVideoModel*>())
{
    createIfNotExistsFile(getYoutubeCachePath(), "[]");

    readYoutubeItems();

    connect(m_apiService, &AnilibriaApiService::allYoutubeItemReceived, this, &YoutubeViewModel::receiveYoutubeVideos);
}

int YoutubeViewModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_youtubeVideos->count();
}

QVariant YoutubeViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto video = m_youtubeVideos->at(index.row());

    switch (role) {
        case IdRole: {
            return QVariant(video->id());
        }
        case TitleRole: {
            return QVariant(video->title());
        }
        case ImageRole: {
            return QVariant(AnilibriaImagesPath + video->image());
        }
        case VideoUrlRole: {
            return QVariant("https://www.youtube.com/watch?v=" + video->videoIdentifier());
        }
        case TimestampRole: {
            return QVariant(video->timestamp());
        }
    }

    return QVariant();
}

QHash<int, QByteArray> YoutubeViewModel::roleNames() const
{
    return {
        {
            IdRole,
            "id"
        },
        {
            TitleRole,
            "title"
        },
        {
            ImageRole,
            "image"
        },
        {
            VideoUrlRole,
            "videoUrl"
        },
        {
            TimestampRole,
            "timestamp"
        }
    };
}

void YoutubeViewModel::synchronize()
{
    m_apiService->getYoutubeVideos();
}

void YoutubeViewModel::createIfNotExistsFile(QString path, QString defaultContent) const
{
    if (QFile::exists(path)) return;

    QFile createReleasesCacheFile(path);
    createReleasesCacheFile.open(QFile::WriteOnly | QFile::Text);
    createReleasesCacheFile.write(defaultContent.toUtf8());
    createReleasesCacheFile.close();
}

QString YoutubeViewModel::getYoutubeCachePath() const
{
    if (IsPortable) {
        return QDir::currentPath() + "/youtube.cache";
    } else {
        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/youtube.cache";
    }
}

void YoutubeViewModel::saveYoutubeItems(const QString &youtubeItems)
{
    QFile file(getYoutubeCachePath());
    if (!file.open(QFile::WriteOnly | QFile::Text)) return;

    file.write(youtubeItems.toUtf8());

    file.close();
}

void YoutubeViewModel::readYoutubeItems()
{
    QFile file(getYoutubeCachePath());
    if (!file.open(QFile::ReadOnly | QFile::Text)) return;

    auto jsonData = file.readAll();
    file.close();

    beginResetModel();

    m_youtubeVideos->clear();

    auto jsonDocument = QJsonDocument::fromJson(jsonData);
    auto rootObject = jsonDocument.object();
    if (!rootObject.contains("data")) return;

    auto dataObject = rootObject["data"].toObject();
    if (!dataObject.contains("items")) return;

    auto items = dataObject["items"].toArray();

    foreach (auto item, items) {
        auto object = item.toObject();
        auto model = new YoutubeVideoModel();
        model->readFromJson(object);

        m_youtubeVideos->append(model);
    }

    endResetModel();
}

void YoutubeViewModel::receiveYoutubeVideos(const QString &data)
{
    saveYoutubeItems(data);

    readYoutubeItems();

    emit synchronizeCompleted();
}

