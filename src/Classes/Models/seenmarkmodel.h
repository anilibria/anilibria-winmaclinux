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

#ifndef SEENMARKMODEL_H
#define SEENMARKMODEL_H

#include <QJsonObject>
#include <QJsonValue>

class SeenMarkModel
{

private:
    int m_Id;
    int m_SeriaId;

public:
    int id() const { return m_Id; }
    int seriaId() const { return m_SeriaId; }

    void setId(const int id) noexcept;
    void setSeriaId(const int seriaId) noexcept;

    void writeToJson(QJsonObject &json) const noexcept;
    void readFromJson(QJsonValue &json);

};

#endif // SEENMARKMODEL_H
