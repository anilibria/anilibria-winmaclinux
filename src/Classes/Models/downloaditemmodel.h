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

#ifndef DOWNLOADITEMMODEL_H
#define DOWNLOADITEMMODEL_H

#include <QString>
#include <QJsonObject>

class DownloadItemModel
{

private:
    int m_ReleaseId;
    int m_VideoId;
    int m_Quality;
    bool m_Downloaded;
    QString m_DownloadedUrl;

public:
    DownloadItemModel();

    int releaseId() const { return m_ReleaseId; }
    void setReleaseId(int releaseId) noexcept;

    int videoId() const { return m_VideoId; }
    void setVideoId(int videoId) noexcept;

    int quality() const { return m_Quality; }
    void setQuality(int quality) noexcept;

    bool downloaded() const { return m_Downloaded; }
    void setDownloaded(bool downloaded) noexcept;

    QString downloadedUrl() const { return m_DownloadedUrl; }
    void setDownloadedUrl(const QString& downloadedUrl) noexcept;

    void readFromJsonObject(const QJsonObject &jsonObject) noexcept;
    void writeToJsonObject(QJsonObject &jsonObject) noexcept;

};

#endif // DOWNLOADITEMMODEL_H
