#include <QVariant>
#include <QJsonObject>
#include <QJsonDocument>
#include "fullreleasemodel.h"
#include "globalconstants.h"

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
    if (poster.contains("https://")) {
        m_Poster = poster;
    } else {
        m_Poster = AnilibriaImagesPath + poster;
    }
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

void FullReleaseModel::setTorrents(const QString &torrents) noexcept
{
    m_Torrents = torrents;
}

void FullReleaseModel::setVideos(const QString &videos) noexcept
{
    m_Videos = videos;
}

void FullReleaseModel::setTimestamp(const int timestamp) noexcept
{
    m_Timestamp = timestamp;
}

void FullReleaseModel::setType(const QString &type) noexcept
{
    m_Type = type;
}

void FullReleaseModel::writeToJson(QJsonObject &json) const noexcept
{
    json["id"] = m_Id;
    json["title"] = m_Title;
    json["code"] = m_Code;
    json["originalName"] = m_OriginalName;
    json["rating"] = m_Rating;
    json["series"] = m_Series;
    json["status"] = m_Status;
    json["type"] = m_Type;
    json["timestamp"] = m_Timestamp;
    json["year"] = m_Year;
    json["season"] = m_Season;
    json["countTorrents"] = m_CountTorrents;
    json["countVideos"] = m_CountVideos;
    json["description"] = m_Description;
    json["announce"] = m_Announce;
    json["genres"] = m_Genres;
    json["poster"] = m_Poster;
    json["voices"] = m_Voices;
    json["torrents"] = m_Torrents;
    json["videos"] = m_Videos;
}

void FullReleaseModel::readFromJson(QJsonValue &json)
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
    setTorrents(json["torrents"].toString());
    setVideos(json["videos"].toString());
}
bool FullReleaseModel::operator== (const FullReleaseModel &comparedModel) noexcept
{
    return m_Id == comparedModel.id();
}
