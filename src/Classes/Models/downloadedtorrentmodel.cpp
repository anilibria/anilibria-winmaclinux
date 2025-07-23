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

void DownloadedTorrentModel::addFile(bool isDownloaded, int downloadPercent, const QString &downloadedPath, int64_t size)
{
    m_files.append(std::make_tuple(isDownloaded, downloadPercent, downloadedPath, size));
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

QList<std::tuple<bool, int, QString, int64_t> > DownloadedTorrentModel::getOriginalFiles() const noexcept
{
    return m_files;
}

int64_t DownloadedTorrentModel::getSizeAllFiles() const noexcept
{
    int64_t resultSize = 0;

    foreach (auto file, m_files) {
        resultSize += std::get<3>(file);
    }

    return resultSize;
}

QString DownloadedTorrentModel::getDownloadedFile(int index) const noexcept
{
    if (index >= m_files.size()) return "";

    auto file = m_files[index];
    auto isDownloaded = std::get<0>(file);
    if (!isDownloaded) return "";

    return std::get<2>(file);
}

void DownloadedTorrentModel::resetData()
{
    m_files.clear();
    m_downloadPath.clear();
    m_title.clear();
    m_poster.clear();
    m_torrentPoster.clear();
}
