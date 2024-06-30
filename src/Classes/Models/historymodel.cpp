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

void HistoryModel::readFromJson(const QJsonValue &json)
{
    setId(json["id"].toInt());
    setTimestamp(json["timestamp"].toInt());
    setWatchTimestamp(json["watchTimestamp"].toInt());
}
