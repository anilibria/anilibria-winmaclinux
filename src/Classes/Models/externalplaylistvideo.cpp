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

#include "externalplaylistvideo.h"

ExternalPlaylistVideo::ExternalPlaylistVideo()
    : m_Order(0)
    , m_Address("")
    , m_Name("")
{
}

void ExternalPlaylistVideo::setOrder(int order) noexcept
{
    m_Order = order;
}

void ExternalPlaylistVideo::setAddress(const QString &address) noexcept
{
    m_Address = address;
}

void ExternalPlaylistVideo::setName(const QString &name) noexcept
{
    m_Name = name;
}

QString ExternalPlaylistVideo::exportToM3U() noexcept
{
    return "#EXTINF:-1, " + m_Name + "\n" + m_Address + "\n\n";
}

QString ExternalPlaylistVideo::exportToMPCPL() noexcept
{
    QString order = QString::number(m_Order);
    return order + ",type,0\n" + order + ",label," + m_Name + "\n" + order + ",filename," + m_Address + "\n";
}

bool ExternalPlaylistVideo::operator==(const ExternalPlaylistVideo &comparedModel) noexcept
{
    return m_Order == comparedModel.order();
}
