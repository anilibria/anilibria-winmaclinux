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

#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QJsonObject>
#include <QJsonValue>

class HistoryModel
{

private:
    int m_Id;
    int m_Timestamp;
    int m_WatchTimestamp;

public:
    HistoryModel();

    int id() const { return m_Id; }
    int timestamp() const { return m_Timestamp; }
    int watchTimestamp() const { return m_WatchTimestamp; }

    void setId(const int id) noexcept;
    void setTimestamp(const int timestamp) noexcept;
    void setWatchTimestamp(const int watchTimestamp) noexcept;

    void writeToJson(QJsonObject &json) const noexcept;
    void readFromJson(QJsonValue &json);

};

#endif // HISTORYMODEL_H
