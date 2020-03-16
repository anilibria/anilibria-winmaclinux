#include <QVariant>
#include <QJsonObject>
#include <QJsonDocument>
#include "fullreleasemodel.h"
#include "globalconstants.h"

QString FullReleaseModel::title() const
{
    return m_Title;
}

void FullReleaseModel::setTitle(const QString &title)
{
    m_Title = title;
}

QString FullReleaseModel::status() const
{
    return m_Status;
}

void FullReleaseModel::setStatus(const QString &status)
{
    m_Status = status;
}

QString FullReleaseModel::year() const
{
    return m_Year;
}

void FullReleaseModel::setYear(const QString &year)
{
    m_Year = year;
}

QString FullReleaseModel::poster() const
{
    return m_Poster;
}

void FullReleaseModel::setPoster(const QString &poster)
{
    if (poster.contains("https://")) {
        m_Poster = poster;
    } else {
        m_Poster = AnilibriaImagesPath + poster;
    }
}

QString FullReleaseModel::description() const
{
    return m_Description;
}

void FullReleaseModel::setDescription(const QString &description)
{
    m_Description = description;
}

QString FullReleaseModel::releaseType() const
{
    return m_Type;
}

void FullReleaseModel::setReleaseType(const QString &releaseType)
{
    m_Type = releaseType;
}

QString FullReleaseModel::genres() const
{
    return m_Genres;
}

void FullReleaseModel::setGenres(const QString &genres)
{
    m_Genres = genres;
}

QString FullReleaseModel::voicers() const
{
    return m_Voices;
}

void FullReleaseModel::setVoicers(const QString &voicers)
{
    m_Voices = voicers;
}

QString FullReleaseModel::season() const
{
    return m_Season;
}

void FullReleaseModel::setSeason(const QString &season)
{
    m_Season = season;
}

QString FullReleaseModel::series() const
{
    return m_Series;
}

void FullReleaseModel::setSeries(const QString &series)
{
    m_Series = series;
}

QString FullReleaseModel::code() const
{
    return m_Code;
}

void FullReleaseModel::setCode(const QString &code)
{
    m_Code = code;
}

int FullReleaseModel::id() const
{
    return m_Id;
}

void FullReleaseModel::setId(const int id)
{
    m_Id = id;
}

int FullReleaseModel::countOnlineVideos() const
{
    return m_CountVideos;
}

void FullReleaseModel::setCountOnlineVideos(const int countOnlineVideos)
{
    m_CountVideos = countOnlineVideos;
}

int FullReleaseModel::countTorrents() const
{
    return m_CountTorrents;
}

void FullReleaseModel::setCountTorrents(const int countTorrents)
{
    m_CountTorrents = countTorrents;
}

QString FullReleaseModel::announce() const
{
    return m_Announce;
}

void FullReleaseModel::setAnnounce(const QString &announce)
{
    m_Announce = announce;
}

QString FullReleaseModel::originalName() const
{
    return m_OriginalName;
}

void FullReleaseModel::setOriginalName(const QString &originalName)
{
    m_OriginalName = originalName;
}

int FullReleaseModel::rating() const
{
    return m_Rating;
}

void FullReleaseModel::setRating(const int rating)
{
    m_Rating = rating;
}

QString FullReleaseModel::torrents() const
{
    return m_Torrents;
}

void FullReleaseModel::setTorrents(const QString &torrents)
{
    m_Torrents = torrents;
}

QString FullReleaseModel::videos() const
{
    return m_Videos;
}

void FullReleaseModel::setVideos(const QString &videos)
{
    m_Videos = videos;
}

int FullReleaseModel::timestamp() const
{
    return m_Timestamp;
}

void FullReleaseModel::setTimestamp(const int timestamp)
{
    m_Timestamp = timestamp;
}

QString FullReleaseModel::type() const
{
    return m_Type;
}

void FullReleaseModel::setType(const QString &type)
{
    m_Type = type;
}

void FullReleaseModel::writeToJson(QJsonObject &json) const
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

bool FullReleaseModel::operator==(const FullReleaseModel &comparedModel)
{
    return m_Id == comparedModel.id();
}
