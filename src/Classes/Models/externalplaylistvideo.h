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

#ifndef EXTERNALPLAYLISTVIDEO_H
#define EXTERNALPLAYLISTVIDEO_H

#include <QString>

class ExternalPlaylistVideo
{

private:
    int m_Order;
    QString m_Address;
    QString m_Name;

public:
    ExternalPlaylistVideo();

    int order() const { return m_Order; }
    QString address() const { return m_Address; }
    QString name() const { return m_Name; }

    void setOrder(const int order) noexcept;
    void setAddress(const QString& address) noexcept;
    void setName(const QString& name) noexcept;

    QString exportToM3U() noexcept;
    QString exportToMPCPL() noexcept;

    bool operator== (const ExternalPlaylistVideo &comparedModel) noexcept;
};

#endif // EXTERNALPLAYLISTVIDEO_H
