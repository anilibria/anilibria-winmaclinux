#include "youtubevideomodel.h"

YoutubeVideoModel::YoutubeVideoModel() :
    m_Id(0),
    m_Title(""),
    m_Image(""),
    m_VideoIdentifier(""),
    m_Timestamp(0)
{

}

void YoutubeVideoModel::setId(const int id) noexcept
{
    m_Id = id;
}

void YoutubeVideoModel::setTitle(const QString& title) noexcept
{
    m_Title = title;
}

void YoutubeVideoModel::setImage(const QString& image) noexcept
{
    m_Image = image;
}

void YoutubeVideoModel::setVideoIdentifier(const QString& videoIdentifier) noexcept
{
    m_VideoIdentifier = videoIdentifier;
}

void YoutubeVideoModel::setTimestamp(const int timestamp) noexcept
{
    m_Timestamp = timestamp;
}

void YoutubeVideoModel::writeToJson(QJsonObject &json) const noexcept
{
    json["id"] = m_Id;
    json["title"] = m_Title;
    json["vid"] = m_VideoIdentifier;
    json["timestamp"] = m_Timestamp;
    json["image"] = m_Image;
}

void YoutubeVideoModel::readFromJson(QJsonValue &json)
{
    setId(json["id"].toInt());
    setTitle(json["title"].toString());
    setVideoIdentifier(json["vid"].toString());
    setImage(json["image"].toString());
    setTimestamp(json["timestamp"].toInt());
}
