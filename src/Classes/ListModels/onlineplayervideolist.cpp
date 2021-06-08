#include "onlineplayervideolist.h"

OnlinePlayerVideoList::OnlinePlayerVideoList(QObject *parent) : QAbstractListModel(parent),
    m_videos(new QVector<OnlineVideoModel*>())
{

}

int OnlinePlayerVideoList::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_videos->size();
}

QVariant OnlinePlayerVideoList::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto video = m_videos->at(index.row());

    switch (role) {
        case IdRole: {
            return QVariant(video->id());
        }
        case TitleRole: {
            return QVariant(video->title());
        }
        case LinkSdRole: {
            return QVariant(video->sd());
        }
        case LinkHdRole: {
            return QVariant(video->hd());
        }
        case LinkFullHdRole: {
            return QVariant(video->fullhd());
        }
        case LinkSourceSdRole: {
            return QVariant(video->sourcehd());
        }
        case LinkSourceHdRole: {
            return QVariant(video->sourcesd());
        }
        case ReleaseIdRole: {
            return QVariant("");
        }
        case ReleasePosterRole: {
            return QVariant("");
        }
        case IsSeenRole: {
            return QVariant(false);
        }
        case IsGroupRole: {
            return QVariant(false);
        }
    }

    return QVariant();
}

QHash<int, QByteArray> OnlinePlayerVideoList::roleNames() const
{
    return {
        {
            IdRole,
            "id"
        },
        {
            TitleRole,
            "title"
        },
        {
            LinkSdRole,
            "sd"
        },
        {
            LinkHdRole,
            "hd"
        },
        {
            LinkFullHdRole,
            "fullhd"
        },
        {
            LinkSourceSdRole,
            "srcSd"
        },
        {
            LinkSourceHdRole,
            "srcSd"
        },
        {
            IsSeenRole,
            "isSeen"
        },
        {
            IsGroupRole,
            "isGroup"
        },
        {
            ReleaseIdRole,
            "releaseId"
        },
        {
            ReleasePosterRole,
            "releasePoster"
        }
    };
}

OnlineVideoModel *OnlinePlayerVideoList::getVideoAtIndex(int index) const noexcept
{
    if (index >= m_videos->count()) return nullptr;

    return m_videos->at(index);
}

void OnlinePlayerVideoList::setVideosFromSingleList(const QString &json) noexcept
{
    beginResetModel();

    m_videos->clear();

    auto document = QJsonDocument::fromJson(json.toUtf8());
    auto videosArray = document.array();

    foreach (auto video, videosArray) {
        auto videoModel = new OnlineVideoModel();
        videoModel->readFromApiModel(video.toObject());
        m_videos->append(videoModel);
    }

    endResetModel();
}

void OnlinePlayerVideoList::setVideosFromMultipleList(const QStringList &json) noexcept
{
    if (json.count() > 0) {

    }
}
