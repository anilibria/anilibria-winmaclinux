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

#include "youtubevideomodel.h"

YoutubeVideoModel::YoutubeVideoModel() :
    m_Id(0),
    m_Title(""),
    m_Image(""),
    m_VideoIdentifier(""),
    m_Timestamp(0)
{

}

void YoutubeVideoModel::setId(const int id) noexcept
{
    m_Id = id;
}

void YoutubeVideoModel::setTitle(const QString& title) noexcept
{
    m_Title = title;
}

void YoutubeVideoModel::setImage(const QString& image) noexcept
{
    m_Image = image;
}

void YoutubeVideoModel::setVideoIdentifier(const QString& videoIdentifier) noexcept
{
    m_VideoIdentifier = videoIdentifier;
}

void YoutubeVideoModel::setTimestamp(const int timestamp) noexcept
{
    m_Timestamp = timestamp;
}

void YoutubeVideoModel::writeToJson(QJsonObject &json) const noexcept
{
    json["id"] = m_Id;
    json["title"] = m_Title;
    json["vid"] = m_VideoIdentifier;
    json["timestamp"] = m_Timestamp;
    json["image"] = m_Image;
}

void YoutubeVideoModel::readFromJson(QJsonObject &json)
{
    setId(json["id"].toInt());
    setTitle(json["title"].toString());
    setVideoIdentifier(json["vid"].toString());
    setImage(json["image"].toString());
    setTimestamp(json["timestamp"].toInt());
}
