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

#ifndef RELEASETORRENTMODEL_H
#define RELEASETORRENTMODEL_H

#include <QtCore>

class ReleaseTorrentModel
{

private:
    int m_Id;
    QString m_Hash;
    QString m_Quality;
    QString m_Series;
    long m_Size;
    QString m_Url;
public:
    ReleaseTorrentModel();

    void readFromApiModel(const QJsonObject &jsonObject);

    void writeToJson(QJsonObject &json) const noexcept;

    int id() const { return m_Id; }
    void setId(const int id) noexcept;

    QString hash() const { return m_Hash; }
    void setHash(const QString &hash) noexcept;

    QString quality() const { return m_Quality; }
    void setQuality(const QString &quality) noexcept;

    QString series() const { return m_Series; }
    void setSeries(const QString &series) noexcept;

    long size() const { return m_Size; }
    void setSize(const long size) noexcept;

    QString url() const { return m_Url; }
    void setUrl(const QString &url) noexcept;
};

#endif // RELEASETORRENTMODEL_H
