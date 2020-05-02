#include <QJsonObject>
#include <QJsonObject>
#include <QJsonDocument>
#include "seenmodel.h"



void SeenModel::setId(const int id) noexcept
{
    m_Id = id;
}

void SeenModel::setVideoId(const int videoId) noexcept
{
    m_VideoId = videoId;
}

void SeenModel::setVideoPosition(const double videoPosition) noexcept
{
    m_VideoPosition = videoPosition;
}

void SeenModel::setTimestamp(const int timestamp) noexcept
{
    m_Timestamp = timestamp;
}

void SeenModel::writeToJson(QJsonObject &json) const noexcept
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
