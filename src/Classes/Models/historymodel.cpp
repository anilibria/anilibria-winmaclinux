#include "historymodel.h"

HistoryModel::HistoryModel() : m_Id(0), m_Timestamp(0), m_WatchTimestamp(0)
{

}

int HistoryModel::id() const
{
    return m_Id;
}

int HistoryModel::timestamp() const
{
    return m_Timestamp;
}

int HistoryModel::watchTimestamp() const
{
    return m_WatchTimestamp;
}

void HistoryModel::setId(int id)
{
    m_Id = id;
}

void HistoryModel::setTimestamp(int timestamp)
{
    m_Timestamp = timestamp;
}

void HistoryModel::setWatchTimestamp(int watchTimestamp)
{
    m_WatchTimestamp = watchTimestamp;
}

void HistoryModel::writeToJson(QJsonObject &json) const
{
    json["id"] = m_Id;
    json["timestamp"] = m_Timestamp;
    json["watchTimestamp"] = m_WatchTimestamp;
}

void HistoryModel::readFromJson(QJsonValue &json)
{
    setId(json["id"].toInt());
    setTimestamp(json["timestamp"].toInt());
    setWatchTimestamp(json["watchTimestamp"].toInt());
}
