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

#include "seenmarkmodel.h"

void SeenMarkModel::setId(int id) noexcept
{
    m_Id = id;
}

void SeenMarkModel::setSeriaId(int seriaId) noexcept
{
    m_SeriaId = seriaId;
}

void SeenMarkModel::writeToJson(QJsonObject &json) const noexcept
{
    json["id"] = m_Id;
    json["seriaId"] = m_SeriaId;
}

void SeenMarkModel::readFromJson(QJsonValue &json)
{
    setId(json["id"].toInt());
    setSeriaId(json["seriaId"].toInt());
}
