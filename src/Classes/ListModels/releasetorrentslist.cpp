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

#include <QJsonDocument>
#include <QJsonArray>
#include "releasetorrentslist.h"
#include "../../globalhelpers.h"

ReleaseTorrentsList::ReleaseTorrentsList(QObject *parent) : QAbstractListModel(parent)
{

}

int ReleaseTorrentsList::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_torrents->size();
}

QVariant ReleaseTorrentsList::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto torrent = m_torrents->at(index.row());

    switch (role) {
        case IdRole: {
            return QVariant(torrent->id());
        }
        case SizeRole: {
            return QVariant(getReadableSize(torrent->size()));
        }
        case QualityRole: {
            return QVariant(torrent->quality());
        }
        case SeriesRole: {
            return QVariant(torrent->series());
        }
        case UrlRole: {
            return QVariant(torrent->url());
        }
        case IdentifierRole: {
            return QVariant(index.row());
        }
        case TimeCreationRole: {
            QDateTime timestamp;
            timestamp.setSecsSinceEpoch(torrent->ctime());
            auto date = timestamp.date();
            auto dateAsString = getLeadingZeroDigit(date.day()) + "." + getLeadingZeroDigit(date.month()) + "." + getLeadingZeroDigit(date.year());
            auto time = timestamp.time();
            auto timeAsString = getLeadingZeroDigit(time.hour()) + ":" + getLeadingZeroDigit(time.minute());
            return QVariant(dateAsString + " " + timeAsString);
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ReleaseTorrentsList::roleNames() const
{
    return {
        {
            IdRole,
            "releaseid"
        },
        {
            SizeRole,
            "size"
        },
        {
            QualityRole,
            "quality"
        },
        {
            SeriesRole,
            "series"
        },
        {
            UrlRole,
            "url"
        },
        {
            IdentifierRole,
            "identifier"
        },
        {
            TimeCreationRole,
            "timecreation"
        }
    };
}

void ReleaseTorrentsList::loadTorrentsFromJson(const QString &json)
{
    beginResetModel();

    m_torrents->clear();

    if (json.isEmpty()) return;

    auto jsonDocument = QJsonDocument::fromJson(json.toUtf8());
    auto jsonArray = jsonDocument.array();
    foreach (auto item, jsonArray) {
        auto torrent = new ReleaseTorrentModel();
        torrent->readFromApiModel(item.toObject());
        m_torrents->append(torrent);
    }

    endResetModel();
}

QString ReleaseTorrentsList::getReadableSize(long long size) const noexcept
{
    QList<QString> sizes;
    sizes.append("B");
    sizes.append("KB");
    sizes.append("MB");
    sizes.append("GB");
    sizes.append("TB");

    int order = 0;
    while (size >= 1024 && order < 4) {
        order++;
        size = size / 1024;
    }

    auto stringSize = QString::number(size);
    QString result;
    result.append(stringSize);
    result.append(" ");
    result.append(sizes[order]);
    return result;
}
