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

#include "dowloadedtorrentslistmodel.h"

DowloadedTorrentsListModel::DowloadedTorrentsListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

void DowloadedTorrentsListModel::setup(QSharedPointer<QList<DownloadedTorrentModel *> > downloadTorrents)
{
    m_downloadedTorrents = downloadTorrents;
}

void DowloadedTorrentsListModel::refresh() noexcept
{
    beginResetModel();

    endResetModel();
}

int DowloadedTorrentsListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_downloadedTorrents->size();
}

QVariant DowloadedTorrentsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto item = m_downloadedTorrents->at(index.row());

    switch (role) {
        case ReleaseIdRole: {
            return QVariant(item->releaseId());
        }
        case PosterRole: {
            return QVariant(item->poster());
        }
        case TitleRole: {
            return QVariant(item->title());
        }
        case TorrentTitleRole: {
            return QVariant(item->torrentPoster());
        }
        case FilesCountRole: {
            return QVariant(item->countFiles());
        }
        case FilesDownloadedRole: {
            return QVariant(item->countDownloadedFiles());
        }
        case IdentifierRole: {
            return QVariant(index.row());
        }
        case TorrentPathRole: {
            return QVariant(item->downloadPath());
        }
        default: return QVariant();
    }
}

QHash<int, QByteArray> DowloadedTorrentsListModel::roleNames() const
{
    return {
        {
            ReleaseIdRole,
            "releaseId"
        },
        {
            PosterRole,
            "poster"
        },
        {
            TitleRole,
            "title"
        },
        {
            TorrentTitleRole,
            "torrentTitle"
        },
        {
            FilesCountRole,
            "filesCount"
        },
        {
            FilesDownloadedRole,
            "filesDownloaded"
        },
        {
            IdentifierRole,
            "identifier"
        },
        {
            TorrentPathRole,
            "torrentPath"
        }
    };
}
