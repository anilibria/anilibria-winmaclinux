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

#include "releasemodel.h"
#include "onlinevideomodel.h"
#include "releasetorrentmodel.h"

ReleaseModel::ReleaseModel()
{
    m_Videos = QList<OnlineVideoModel>();
    m_Torrents = QList<ReleaseTorrentModel>();
    m_Timestamp = "0";
    m_Rating = 0;
    m_Status = "";
    m_Announce = "";
}

ReleaseModel::~ReleaseModel()
{
    m_Videos.clear();
    m_Torrents.clear();
}

void ReleaseModel::writeToApiModel(QJsonObject& json) const noexcept
{
    json["id"] = m_Id;
    json["code"] = m_Code;
    json["poster"] = m_Poster;
    json["series"] = m_Series;
    json["status"] = m_Status;
    json["last"] = m_Timestamp;
    json["type"] = m_Type;
    json["year"] = m_Year;
    json["description"] = m_Description;
    QJsonObject rating;
    rating["rating"] = m_Rating;
    json["favorite"] = rating;
    json["season"] = m_Season;
    json["announce"] = m_Announce;
    QJsonArray namesArray = QJsonArray();
    foreach(const QString & name, m_Names) namesArray.append(QJsonValue(name));
    json["names"] = namesArray;
    QJsonArray voicesArray = QJsonArray();
    foreach(const QString & voice, m_Voices) voicesArray.append(QJsonValue(voice));
    json["voices"] = voicesArray;
    QJsonArray genresArray = QJsonArray();
    foreach(const QString & genre, m_Genres) genresArray.append(QJsonValue(genre));
    json["genres"] = genresArray;

    QJsonArray playlistArray = QJsonArray();
    foreach(OnlineVideoModel video, m_Videos) {
        QJsonObject jsonObject;
        video.writeToJson(jsonObject);
        playlistArray.append(jsonObject);
    }
    json["playlist"] = playlistArray;


    QJsonArray torrentsArray = QJsonArray();
    foreach(ReleaseTorrentModel torrent, m_Torrents) {
        QJsonObject jsonObject;
        torrent.writeToJson(jsonObject);
        torrentsArray.append(jsonObject);
    }
    json["torrents"] = torrentsArray;
}

void ReleaseModel::readFromApiModel(const QJsonObject &jsonObject)
{
    m_Id = jsonObject.value("id").toInt();
    m_Code = jsonObject.value("code").toString();
    m_Poster = jsonObject.value("poster").toString();
    m_Series = jsonObject.value("series").toString();
    m_Status = jsonObject.value("status").toString();
    m_Timestamp = jsonObject.value("last").toString();
    m_Type = jsonObject.value("type").toString();
    m_Year = jsonObject.value("year").toString();
    m_Description = jsonObject.value("description").toString();
    m_Rating = jsonObject.value("favorite").toObject().value("rating").toInt(0);
    m_Season = jsonObject.value("season").toString();
    m_Announce = jsonObject.value("announce").toString();
    auto names = jsonObject.value("names").toArray();
    foreach(const QJsonValue & name, names) m_Names.append(name.toString());
    m_Title = m_Names.first();

    auto voicers = jsonObject.value("voices").toArray();
    foreach(const QJsonValue & voicer, voicers) m_Voices.append(voicer.toString());

    auto genres = jsonObject.value("genres").toArray();
    foreach(const QJsonValue & genre, genres) m_Genres.append(genre.toString());

    auto videos = jsonObject.value("playlist").toArray();
    foreach(QJsonValue video, videos) {
        auto videoModel = OnlineVideoModel();
        videoModel.readFromApiModel(video.toObject());
        m_Videos.append(videoModel);
    }

    auto torrents = jsonObject.value("torrents").toArray();
    foreach(QJsonValue torrent, torrents) {
        auto torrentModel = ReleaseTorrentModel();
        torrentModel.readFromApiModel(torrent.toObject());
        m_Torrents.append(torrentModel);
    }
}

void ReleaseModel::readFromJson(const QJsonObject &json)
{
    m_Id = json["id"].toInt();
    m_Code = json["code"].toString();
    m_Poster = json["poster"].toString();
    m_Series = json["series"].toString();
    m_Status = json["status"].toString();
    m_Timestamp = json["last"].toString();
    m_Type = json["type"].toString();
    m_Year = json["year"].toString();
    m_Description = json["description"].toString();
    m_Rating = json["rating"].toInt();
    m_Title = json["title"].toString();
    m_Season = json["season"].toString();
    m_Announce = json["announce"].toString();

    auto namesArray = json["names"].toArray();
    foreach(const QJsonValue & name, namesArray) m_Names.append(name.toString());

    auto voicers = json["voices"].toArray();
    foreach(const QJsonValue & voicer, voicers) m_Voices.append(voicer.toString());

    auto genres = json["genres"].toArray();
    foreach(const QJsonValue & genre, genres) m_Genres.append(genre.toString());

    auto videos = json["playlist"].toArray();
    foreach(QJsonValue video, videos) {
        auto videoModel = OnlineVideoModel();
        videoModel.readFromApiModel(video.toObject());
        m_Videos.append(videoModel);
    }

    auto torrents = json["torrents"].toArray();
    foreach(QJsonValue torrent, torrents) {
        auto torrentModel = ReleaseTorrentModel();
        torrentModel.readFromApiModel(torrent.toObject());
        m_Torrents.append(torrentModel);
    }
}

void ReleaseModel::writeToJson(QJsonObject &json) const noexcept
{
    json["id"] = m_Id;
    json["code"] = m_Code;
    json["poster"] = m_Poster;
    json["series"] = m_Series;
    json["status"] = m_Status;
    json["last"] = m_Timestamp;
    json["type"] = m_Type;
    json["year"] = m_Year;
    json["description"] = m_Description;
    json["rating"] = m_Rating;
    json["title"] = m_Names.first();
    json["season"] = m_Season;
    json["announce"] = m_Announce;
    QJsonArray namesArray = QJsonArray();
    foreach(const QString & name, m_Names) namesArray.append(QJsonValue(name));
    json["names"] = namesArray;
    QJsonArray voicesArray = QJsonArray();
    foreach(const QString & voice, m_Voices) voicesArray.append(QJsonValue(voice));
    json["voices"] = voicesArray;
    QJsonArray genresArray = QJsonArray();
    foreach(const QString & genre, m_Genres) genresArray.append(QJsonValue(genre));
    json["genres"] = genresArray;

    QJsonArray playlistArray = QJsonArray();
    foreach(OnlineVideoModel video, m_Videos) {
        QJsonObject jsonObject;
        video.writeToJson(jsonObject);
        playlistArray.append(jsonObject);
    }
    json["playlist"] = playlistArray;


    QJsonArray torrentsArray = QJsonArray();
    foreach(ReleaseTorrentModel torrent, m_Torrents) {
        QJsonObject jsonObject;
        torrent.writeToJson(jsonObject);
        torrentsArray.append(jsonObject);
    }
    json["torrents"] = torrentsArray;
}
