#include "onlineplayervideolist.h"

OnlinePlayerVideoList::OnlinePlayerVideoList(QObject *parent) : QAbstractListModel(parent),
    m_videos(new QVector<OnlineVideoModel*>()),
    m_selectedReleaseId(0),
    m_selectedVideoId(0)
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
            return QVariant(video->releaseId());
        }
        case ReleasePosterRole: {
            return QVariant(video->releasePoster());
        }
        case IsSeenRole: {
            return QVariant(false);
        }
        case IsGroupRole: {
            return QVariant(video->isGroup());
        }
        case SelectedRole: {
            return QVariant(video->releaseId() == m_selectedReleaseId && video->order() == m_selectedVideoId);
        }
        case OrderRole: {
            return QVariant(video->order());
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
        },
        {
            SelectedRole,
            "selectedVideo"
        },
        {
            OrderRole,
            "order"
        }
    };
}

OnlineVideoModel *OnlinePlayerVideoList::getVideoAtIndex(int index) const noexcept
{
    if (index >= m_videos->count()) return nullptr;

    return m_videos->at(index);
}

int OnlinePlayerVideoList::getReleaseVideosCount(int releaseId) const noexcept
{
    return std::count_if(m_videos->begin(), m_videos->end(), [releaseId] (OnlineVideoModel* item) { return item->releaseId() == releaseId; });
}

OnlineVideoModel *OnlinePlayerVideoList::getFirstReleaseWithPredicate(std::function<bool(OnlineVideoModel *)> callback, bool isEndDirection) const noexcept
{
    if (isEndDirection) {
        auto searchResult = std::find_if(
            m_videos->crbegin(),
            m_videos->crend(),
            [callback](OnlineVideoModel * video) {
                return callback(video);
            }
        );

        if (searchResult == m_videos->crend()) return nullptr;

        return *searchResult;
    } else {
        auto searchResult = std::find_if(
            m_videos->begin(),
            m_videos->end(),
            [callback](OnlineVideoModel * video) {
                return callback(video);
            }
        );

        if (searchResult == m_videos->end()) return nullptr;

        return *searchResult;
    }
}

void OnlinePlayerVideoList::setVideosFromSingleList(const QString &json, int releaseId, const QString& poster) noexcept
{
    beginResetModel();

    m_videos->clear();

    auto document = QJsonDocument::fromJson(json.toUtf8());
    auto videosArray = document.array();

    foreach (auto video, videosArray) {
        auto videoModel = new OnlineVideoModel();
        videoModel->readFromApiModel(video.toObject());
        videoModel->setReleaseId(releaseId);
        videoModel->setReleasePoster(poster);
        videoModel->setIsGroup(false);
        m_videos->append(videoModel);
    }

    std::sort(
        m_videos->begin(),
        m_videos->end(),
        [](const OnlineVideoModel* first, const OnlineVideoModel* second) {
            return first->id() < second->id();
        }
    );

    QVectorIterator<OnlineVideoModel*> iterator(*m_videos);
    int index = -1;
    while (iterator.hasNext()) {
        auto item = iterator.next();
        index += 1;
        item->setOrder(index);
    }

    endResetModel();
}

inline void swap(QJsonValueRef v1, QJsonValueRef v2)
{
    QJsonValue temp(v1);
    v1 = QJsonValue(v2);
    v2 = temp;
}

void OnlinePlayerVideoList::setVideosFromCinemahall(const QStringList &json, const QList<int> &releases, const QStringList &posters, const QStringList& names) noexcept
{
    if (json.count() != releases.count()) return;
    if (releases.count() != posters.count()) return;
    if (names.count() != posters.count()) return;

    beginResetModel();

    m_videos->clear();

    for (int i = 0; i < json.count(); i++) {
        auto document = QJsonDocument::fromJson(json.value(i).toUtf8());
        auto videosArray = document.array();

        std::sort(videosArray.begin(), videosArray.end(), [](const QJsonValue &left, const QJsonValue &right) {
            return left.toObject()["id"].toInt() < right.toObject()["id"].toInt();
        });

        auto groupModel = new OnlineVideoModel();
        groupModel->setTitle(names.value(i));
        groupModel->setIsGroup(true);
        m_videos->append(groupModel);

        auto release = releases.value(i);
        int index = -1;
        foreach (auto video, videosArray) {
            auto videoModel = new OnlineVideoModel();
            videoModel->readFromApiModel(video.toObject());
            videoModel->setReleaseId(release);
            videoModel->setReleasePoster(posters.value(i));
            videoModel->setIsGroup(false);
            index += 1;
            videoModel->setOrder(index);
            m_videos->append(videoModel);
        }
    }

    endResetModel();
}

void OnlinePlayerVideoList::selectVideo(int releaseId, int videoId) noexcept
{
    if (releaseId == m_selectedReleaseId && videoId == m_selectedVideoId) return;

    int currentIndex = -1;
    int newIndex = -1;
    int currentSelectedReleaseId = m_selectedReleaseId;
    int currentSelectedVideoId = m_selectedVideoId;
    auto currentlySelected = std::find_if(
        m_videos->begin(),
        m_videos->end(),
        [currentSelectedReleaseId, currentSelectedVideoId](OnlineVideoModel * video) {
            return video->releaseId() == currentSelectedReleaseId && video->order() == currentSelectedVideoId;
        }
    );
    if (currentlySelected != m_videos->end()) currentIndex = m_videos->indexOf(*currentlySelected);

    auto newSelected = std::find_if(
        m_videos->begin(),
        m_videos->end(),
        [releaseId, videoId](OnlineVideoModel * video) {
            return video->releaseId() == releaseId && video->order() == videoId;
        }
    );
    newIndex = m_videos->indexOf(*newSelected);

    m_selectedReleaseId = releaseId;
    m_selectedVideoId = videoId;

    emit dataChanged(index(currentIndex), index(currentIndex));
    emit dataChanged(index(newIndex), index(newIndex));
}

int OnlinePlayerVideoList::getVideoIndex(OnlineVideoModel* video) noexcept
{
    return m_videos->indexOf(video);
}

QList<int> OnlinePlayerVideoList::getReleaseIds() noexcept
{
    QList<int> result;

    std::transform(
        m_videos->begin(),
        m_videos->end(),
        std::back_inserter(result),
        [](OnlineVideoModel* video) -> int {
            if (video->isGroup()) return -1;

            return video->releaseId();
        }
    );

    //remove group ids
    result.removeAll(-1);

    QSet<int> set(result.cbegin(), result.cend());

    return set.values();
}
