#include <QJsonDocument>
#include <QJsonArray>
#include "releasetorrentslist.h"

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
    }

    return QVariant();
}

QHash<int, QByteArray> ReleaseTorrentsList::roleNames() const
{
    return {
        {
            IdRole,
            "id"
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
