#include "youtubevideomodel.h"

YoutubeVideoModel::YoutubeVideoModel() :
    m_Id(0),
    m_Title(""),
    m_Image(""),
    m_VideoIdentifier(""),
    m_Timestamp(0)
{

}

int YoutubeVideoModel::id()
{
    return m_Id;
}

QString YoutubeVideoModel::title()
{
    return m_Title;
}

QString YoutubeVideoModel::image()
{
    return m_Image;
}

QString YoutubeVideoModel::videoIdentifier()
{
    return m_VideoIdentifier;
}

int YoutubeVideoModel::timestamp()
{
    return m_Timestamp;
}

void YoutubeVideoModel::setId(int id)
{
    m_Id = id;
}

void YoutubeVideoModel::setTitle(QString title)
{
    m_Title = title;
}

void YoutubeVideoModel::setImage(QString image)
{
    m_Image = image;
}

void YoutubeVideoModel::setVideoIdentifier(QString videoIdentifier)
{
    m_VideoIdentifier = videoIdentifier;
}

void YoutubeVideoModel::setTimestamp(int timestamp)
{
    m_Timestamp = timestamp;
}

void YoutubeVideoModel::writeToJson(QJsonObject &json) const
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
