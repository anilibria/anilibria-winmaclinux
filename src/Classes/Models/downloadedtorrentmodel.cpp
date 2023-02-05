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

#include "downloadedtorrentmodel.h"

DownloadedTorrentModel::DownloadedTorrentModel()
{

}

void DownloadedTorrentModel::addFile(bool isDownloaded, int downloadPercent, const QString &downloadedPath)
{
    m_files.append(std::make_tuple(isDownloaded, downloadPercent, downloadedPath));
}

int DownloadedTorrentModel::countFiles() const noexcept
{
    return m_files.count();
}

int DownloadedTorrentModel::countDownloadedFiles() const noexcept
{
    auto result = 0;
    foreach(auto file, m_files) {
        if (std::get<0>(file)) result++;
    }

    return result;
}

QStringList DownloadedTorrentModel::getFiles() const noexcept
{
    QStringList result;
    foreach (auto file, m_files) {
        result.append(std::get<2>(file));
    }

    return result;
}

void DownloadedTorrentModel::resetData()
{
    m_files.clear();
    m_downloadPath.clear();
    m_title.clear();
    m_poster.clear();
    m_torrentPoster.clear();
}
