#include "releasemodel.h"
#include "onlinevideomodel.h"
#include "releasetorrentmodel.h"

ReleaseModel::ReleaseModel()
{
    m_Videos = QList<OnlineVideoModel>();
    m_Torrents = QList<ReleaseTorrentModel>();
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

void ReleaseModel::writeToJson(QJsonObject &json) const
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

int ReleaseModel::id()
{
    return m_Id;
}

QString ReleaseModel::code()
{
    return m_Code;
}

QString ReleaseModel::series()
{
    return m_Series;
}

QString ReleaseModel::poster()
{
    return m_Poster;
}

QString ReleaseModel::timestamp()
{
    return m_Timestamp;
}

QString ReleaseModel::status()
{
    return m_Status;
}

QString ReleaseModel::type()
{
    return m_Type;
}

QString ReleaseModel::year()
{
    return m_Year;
}

QString ReleaseModel::description()
{
    return m_Description;
}

QString ReleaseModel::season()
{
    return m_Season;
}

QString ReleaseModel::announce()
{
    return m_Announce;
}

QStringList ReleaseModel::genres()
{
    return m_Genres;
}

QStringList ReleaseModel::voices()
{
    return m_Voices;
}

QStringList ReleaseModel::names()
{
    return m_Names;
}

int ReleaseModel::rating()
{
    return m_Rating;
}

QString ReleaseModel::title()
{
    return m_Title;
}

QList<OnlineVideoModel> ReleaseModel::videos()
{
    return m_Videos;
}

QList<ReleaseTorrentModel> ReleaseModel::torrents()
{
    return m_Torrents;
}
