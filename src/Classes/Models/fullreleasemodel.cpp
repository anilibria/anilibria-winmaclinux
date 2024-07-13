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

#include <QVariant>
#include <QJsonObject>
#include <QJsonDocument>
#include "fullreleasemodel.h"

void FullReleaseModel::setTitle(const QString &title) noexcept
{
    m_Title = title;
}

void FullReleaseModel::setStatus(const QString &status) noexcept
{
    m_Status = status;
}

void FullReleaseModel::setYear(const QString &year) noexcept
{
    m_Year = year;
}

void FullReleaseModel::setPoster(const QString &poster) noexcept
{
    m_Poster = poster;
}

void FullReleaseModel::setDescription(const QString &description) noexcept
{
    m_Description = description;
}

void FullReleaseModel::setReleaseType(const QString &releaseType) noexcept
{
    m_Type = releaseType;
}

void FullReleaseModel::setGenres(const QString &genres) noexcept
{
    m_Genres = genres;
}

void FullReleaseModel::setVoicers(const QString &voicers) noexcept
{
    m_Voices = voicers;
}

void FullReleaseModel::setSeason(const QString &season) noexcept
{
    m_Season = season;
}

void FullReleaseModel::setSeries(const QString &series) noexcept
{
    m_Series = series;
}

void FullReleaseModel::setCode(const QString &code) noexcept
{
    m_Code = code;
}

void FullReleaseModel::setId(const int id) noexcept
{
    m_Id = id;
}

void FullReleaseModel::setCountOnlineVideos(const int countOnlineVideos) noexcept
{
    m_CountVideos = countOnlineVideos;
}

void FullReleaseModel::setCountTorrents(const int countTorrents) noexcept
{
    m_CountTorrents = countTorrents;
}

void FullReleaseModel::setAnnounce(const QString &announce) noexcept
{
    m_Announce = announce;
}

void FullReleaseModel::setOriginalName(const QString &originalName) noexcept
{
    m_OriginalName = originalName;
}

void FullReleaseModel::setRating(const int rating) noexcept
{
    m_Rating = rating;
}

void FullReleaseModel::setTimestamp(const int timestamp) noexcept
{
    m_Timestamp = timestamp;
}

void FullReleaseModel::setType(const QString &type) noexcept
{
    m_Type = type;
}

void FullReleaseModel::readFromJson(const QJsonObject &json)
{
    setId(json["id"].toInt());
    setTitle(json["title"].toString());
    setCode(json["code"].toString());
    setOriginalName(json["originalName"].toString());
    setRating(json["rating"].toInt());
    setSeries(json["series"].toString());
    setStatus(json["status"].toString());
    setType(json["type"].toString());
    setTimestamp(json["timestamp"].toInt());
    setYear(json["year"].toString());
    setSeason(json["season"].toString());
    setCountTorrents(json["countTorrents"].toInt());
    setCountOnlineVideos(json["countVideos"].toInt());
    setDescription(json["description"].toString());
    setAnnounce(json["announce"].toString());
    setGenres(json["genres"].toString());
    setPoster(json["poster"].toString());
    setVoicers(json["voices"].toString());
    setIsOngoing(json[m_isOngoingField].toBool());
    setAgeRating(json[m_ageRatingField].toString());
}
bool FullReleaseModel::operator== (const FullReleaseModel &comparedModel) noexcept
{
    return m_Id == comparedModel.id();
}
