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

#include "onlinevideomodel.h"
#include <QJsonObject>
#include <QJsonArray>

OnlineVideoModel::OnlineVideoModel()
{

}

void OnlineVideoModel::readFromApiModel(const QJsonObject &jsonObject)
{
    m_Id = jsonObject.value("id").toInt();
    m_Title = jsonObject.value("title").toString();
    m_SD = jsonObject.value("sd").toString();
    m_HD = jsonObject.value("hd").toString();
    m_FullHD = jsonObject.value("fullhd").toString();
    m_SourceSD = jsonObject.value("srcSd").toString();
    m_SourceHD = jsonObject.value("srcHd").toString();
    m_videoPoster = jsonObject.value("poster").toString();
    if (jsonObject.contains("skips")) {
        auto skips = jsonObject.value("skips").toObject();

        if (skips.contains("ending")) {
            auto endings = skips.value("ending").toArray();
            if (endings.count() == 2) {
                m_endingStartSeconds = endings.first().toInt();
                m_endingEndSeconds = endings.last().toInt();
            }
        }

        if (skips.contains("opening")) {
            auto openings = skips.value("opening").toArray();
            if (openings.count() == 2) {
                m_openingStartSeconds = openings.first().toInt();
                m_openingEndSeconds = openings.last().toInt();
            }
        }
    }
}

void OnlineVideoModel::writeToJson(QJsonObject &json) const noexcept
{
    json["id"] = m_Id;
    json["title"] = m_Title;
    json["sd"] = m_SD;
    json["hd"] = m_HD;
    json["fullhd"] = m_FullHD;
    json["srcSd"] = m_SourceSD;
    json["srcHd"] = m_SourceHD;
    json["poster"] = m_videoPoster;

    auto skipsObject = QJsonObject();

    if (m_endingStartSeconds > -1 && m_endingEndSeconds > -1) {
        auto endings = QJsonArray();
        endings.append(m_endingStartSeconds);
        endings.append(m_endingEndSeconds);
        skipsObject["ending"] = endings;
    }

    if (m_openingStartSeconds > -1 && m_openingEndSeconds > -1) {
        auto openings = QJsonArray();
        openings.append(m_openingStartSeconds);
        openings.append(m_openingEndSeconds);
        skipsObject["opening"] = openings;
    }
    json["skips"] = skipsObject;
}

void OnlineVideoModel::setId(const int id) noexcept
{
    m_Id = id;
}

void OnlineVideoModel::setTitle(const QString &title) noexcept
{
    m_Title = title;
}

void OnlineVideoModel::setSd(const QString &sd) noexcept
{
    m_SD = sd;
}

void OnlineVideoModel::setHd(const QString &hd) noexcept
{
    m_HD = hd;
}

void OnlineVideoModel::setFullHd(const QString &fullhd) noexcept
{
    m_FullHD = fullhd;
}

void OnlineVideoModel::setSourceSd(const QString &sourcesd) noexcept
{
    m_SourceSD = sourcesd;
}

void OnlineVideoModel::setSourceHd(const QString &sourcehd) noexcept
{
    m_SourceHD = sourcehd;
}

void OnlineVideoModel::setReleaseId(int releaseId) noexcept
{
    m_releaseId = releaseId;
}

void OnlineVideoModel::setOrder(int order) noexcept
{
    m_order = order;
}

void OnlineVideoModel::setReleasePoster(QString releasePoster) noexcept
{
    m_releasePoster = releasePoster;
}

void OnlineVideoModel::setIsGroup(bool isGroup) noexcept
{
    m_isGroup = isGroup;
}

void OnlineVideoModel::setVideoPoster(const QString &poster) noexcept
{
    m_videoPoster = poster;
}
