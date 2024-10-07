#include <QUrl>
#include <QFile>
#include <QDesktopServices>
#include "releasetorrentcommonlist.h"
#include "../../globalhelpers.h"

ReleaseTorrentCommonList::ReleaseTorrentCommonList(QObject *parent)
    : QAbstractListModel{parent}
{
    connect(m_networkManager.get(), &QNetworkAccessManager::finished, this, &ReleaseTorrentCommonList::downloadTorrentResponse);
}

int ReleaseTorrentCommonList::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_torrents->size();
}

QVariant ReleaseTorrentCommonList::data(const QModelIndex &index, int role) const
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
            return QVariant(torrent->description());
        }
        case UrlRole: {
            return QVariant(torrent->torrentHost() + torrent->torrentPath());
        }
        case IndexRole: {
            return QVariant(index.row());
        }
        case TimeCreationRole: {
            QDateTime timestamp;
            timestamp.setSecsSinceEpoch(torrent->created());
            auto date = timestamp.date();
            auto dateAsString = getLeadingZeroDigit(date.day()) + "." + getLeadingZeroDigit(date.month()) + "." + getLeadingZeroDigit(date.year());
            auto time = timestamp.time();
            auto timeAsString = getLeadingZeroDigit(time.hour()) + ":" + getLeadingZeroDigit(time.minute());
            return QVariant(dateAsString + " " + timeAsString);
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ReleaseTorrentCommonList::roleNames() const
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
        },
        {
            IndexRole,
            "currentIndex"
        },
        {
            TimeCreationRole,
            "timecreation"
        }
    };
}

void ReleaseTorrentCommonList::loadFromJson(const QList<ApiTorrentModel *> &torrents)
{
    beginResetModel();

    m_torrents->clear();

    if (torrents.isEmpty()) return;

    foreach (auto item, torrents) {
        m_torrents->append(item);
    }

    endResetModel();
}

QString ReleaseTorrentCommonList::getDownloadPath(int index)
{
    auto torrent = m_torrents->at(index);
    return torrent->torrentPath();
}

QString ReleaseTorrentCommonList::getReadableSize(int64_t size) const noexcept
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

void ReleaseTorrentCommonList::downloadTorrentResponse(QNetworkReply *reply)
{
    auto status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (status_code.isValid()){
        auto code = status_code.toInt();
        if (code != 200) {
            emit failureDownloadTorrent();
            return;
        }
    }

    auto byteArray = reply->readAll();
    if (byteArray.size() == 0) {
        emit failureDownloadTorrent();
        return;
    }

    auto uuid = QUuid::createUuid();
    auto randomName = uuid.toString().replace("-", "").replace("{", "").replace("}", "").replace("[", "").replace("]", "");
    auto randomTorrentPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/" + randomName + ".torrent";
    QFile torrentFile(randomTorrentPath);
    torrentFile.open(QFile::WriteOnly);
    torrentFile.write(byteArray);
    torrentFile.close();

#ifdef Q_OS_WIN
    QDesktopServices::openUrl("file:///" + randomTorrentPath);
#else
    QDesktopServices::openUrl("file://" + randomTorrentPath);
#endif
}
