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

#include "releasetorrentmodel.h"

ReleaseTorrentModel::ReleaseTorrentModel()
{

}

void ReleaseTorrentModel::readFromApiModel(const QJsonObject &jsonObject)
{
    m_Id = jsonObject.value("id").toInt();
    m_Hash = jsonObject.value("hash").toString();
    m_Quality = jsonObject.value("quality").toString();
    m_Series = jsonObject.value("series").toString();
    m_Url = jsonObject.value("url").toString();
    m_Size = jsonObject.value("size").toVariant().toLongLong();
    m_ctime = jsonObject.value("ctime").toVariant().toInt();
}

void ReleaseTorrentModel::writeToJson(QJsonObject &json) const noexcept
{
    json["id"] = m_Id;
    json["hash"] = m_Hash;
    json["quality"] = m_Quality;
    json["series"] = m_Series;
    json["url"] = m_Url;
    json["size"] = m_Size;
    json["ctime"] = m_ctime;
}

void ReleaseTorrentModel::setId(const int id) noexcept
{
    m_Id = id;
}

void ReleaseTorrentModel::setHash(const QString &hash) noexcept
{
    m_Hash = hash;
}

void ReleaseTorrentModel::setQuality(const QString &quality) noexcept
{
    m_Quality = quality;
}

void ReleaseTorrentModel::setSeries(const QString &series) noexcept
{
    m_Series = series;
}

void ReleaseTorrentModel::setSize(const long long size) noexcept
{
    m_Size = size;
}

void ReleaseTorrentModel::setUrl(const QString &url) noexcept
{
    m_Url = url;
}
