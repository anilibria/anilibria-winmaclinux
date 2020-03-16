#include <QJsonObject>
#include <QJsonObject>
#include <QJsonDocument>
#include "seenmodel.h"


int SeenModel::id()
{
    return m_Id;
}

int SeenModel::videoId() const
{
    return m_VideoId;
}

double SeenModel::videoPosition() const
{
    return m_VideoPosition;
}

int SeenModel::timestamp() const
{
    return m_Timestamp;
}

void SeenModel::setId(int id)
{
    m_Id = id;
}

void SeenModel::setVideoId(int videoId)
{
    m_VideoId = videoId;
}

void SeenModel::setVideoPosition(double videoPosition)
{
    m_VideoPosition = videoPosition;
}

void SeenModel::setTimestamp(int timestamp)
{
    m_Timestamp = timestamp;
}

void SeenModel::writeToJson(QJsonObject &json) const
{
    json["id"] = m_Id;
    json["videoId"] = m_VideoId;
    json["videoPosition"] = m_VideoPosition;
    json["timestamp"] = m_Timestamp;
}

void SeenModel::readFromJson(QJsonValue &json)
{
    setId(json["id"].toInt());
    setVideoId(json["videoId"].toInt());
    setVideoPosition(json["videoPosition"].toDouble());
    setTimestamp(json["timestamp"].toInt());
}
