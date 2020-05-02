#include "historymodel.h"

HistoryModel::HistoryModel()
    : m_Id(0)
    , m_Timestamp(0)
    , m_WatchTimestamp(0)
{
}

void HistoryModel::setId(const int id) noexcept
{
    m_Id = id;
}

void HistoryModel::setTimestamp(const int timestamp) noexcept
{
    m_Timestamp = timestamp;
}

void HistoryModel::setWatchTimestamp(const int watchTimestamp) noexcept
{
    m_WatchTimestamp = watchTimestamp;
}

void HistoryModel::writeToJson(QJsonObject &json) const noexcept
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
