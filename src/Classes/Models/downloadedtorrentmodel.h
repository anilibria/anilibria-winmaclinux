/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2021 Roman Vladimirov

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

#ifndef DOWNLOADEDTORRENTMODEL_H
#define DOWNLOADEDTORRENTMODEL_H

#include <QString>
#include <QList>

class DownloadedTorrentModel
{
private:
    int m_releaseId { 0 };
    QString m_downloadPath { "" };
    bool m_allDownloaded { false };
    QString m_title { "" };
    QString m_poster { "" };
    QString m_torrentPoster { "" };
    QList<std::tuple<bool,int, QString>> m_files { QList<std::tuple<bool,int, QString>>() };

public:
    DownloadedTorrentModel();

    int releaseId() const noexcept { return m_releaseId; }
    QString downloadPath() const noexcept { return m_downloadPath; }
    bool allDownloaded() const noexcept { return m_allDownloaded; }
    QString title() const noexcept { return m_title; }
    QString poster() const noexcept { return m_poster; }
    QString torrentPoster() const noexcept { return m_torrentPoster; }

    void setReleaseId(int releaseId) noexcept { m_releaseId = releaseId; }
    void setDownloadPath(const QString& downloadPath) noexcept { m_downloadPath = downloadPath; }
    void setAllDownloaded(bool allDownloaded) noexcept { m_allDownloaded = allDownloaded; }
    void setTitle(const QString& title) noexcept { m_title = title; }
    void setPoster(const QString& poster) noexcept { m_poster = poster; }
    void setTorrentPoster(const QString& torrentPoster) noexcept { m_torrentPoster = torrentPoster; }

    void addFile(bool isDownloaded, int downloadPercent, const QString& downloadedPath);

    int countFiles() const noexcept;
    int countDownloadedFiles() const noexcept;

    void resetData();
};

#endif // DOWNLOADEDTORRENTMODEL_H
