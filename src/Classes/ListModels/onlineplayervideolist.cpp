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

#include "onlineplayervideolist.h"
#include "../../globalconstants.h"

#include <QtGlobal>

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
            return QVariant(m_releaseViewModel->getSeriaSeenMark(video->releaseId(), video->order()));
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
        case PosterRole: {
            return QVariant(AnilibriaImagesPath + video->videoPoster());
        }
        case EndingStartRole: {
            return QVariant(video->endingStartSeconds());
        }
        case EndingEndRole: {
            return QVariant(video->endingEndSeconds());
        }
        case OpeningStartRole: {
            return QVariant(video->openingStartSeconds());
        }
        case OpeningEndRole: {
            return QVariant(video->openingEndSeconds());
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
        },
        {
            PosterRole,
            "poster"
        },
        {
            EndingStartRole,
            "endingStart"
        },
        {
            EndingEndRole,
            "endingEnd"
        },
        {
            OpeningStartRole,
            "openingStart"
        },
        {
            OpeningEndRole,
            "openingEnd"
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

void OnlinePlayerVideoList::setVideosFromDownloadedTorrent(const QStringList &files, int releaseId, const QString& poster) noexcept
{
    beginResetModel();

    m_videos->clear();

    auto release = m_releaseViewModel->getReleaseById(releaseId);
    if (release == nullptr) {
        endResetModel();
        return;
    }

    auto releaseVideos = m_releaseViewModel->getReleaseVideos(releaseId);
    auto videos = fillVideosFrom(releaseVideos, releaseId, poster);

    int iterator = 0;
    foreach (auto file, files) {
        auto videoModel = new OnlineVideoModel();
#ifdef Q_OS_WIN
        auto url = "file:///" + file.replace("\\", "/").replace(" ", "%20");
#else
        auto url = "file://" + file.replace(" ", "%20");
#endif
        videoModel->setFullHd(url);
        videoModel->setHd(url);
        videoModel->setSd(url);
        videoModel->setOrder(iterator);
        videoModel->setReleaseId(releaseId);
        videoModel->setReleasePoster(poster);
        videoModel->setIsGroup(false);
        videoModel->setTitle("Файл торрента " + QString::number(iterator + 1));
        if (iterator < videos.count()) {
            videoModel->setOpeningEndSeconds(videos.value(iterator)->openingEndSeconds());
            videoModel->setOpeningStartSeconds(videos.value(iterator)->openingStartSeconds());
        }
        m_videos->append(videoModel);
        iterator++;
    }

    endResetModel();
}

void OnlinePlayerVideoList::setVideosFromSingleTorrent(const ReleaseTorrentModel& torrent, int releaseId, const QString &poster, int port, const TorrentNotifierViewModel* torrentStream) noexcept
{
    beginResetModel();

    m_videos->clear();

    auto release = m_releaseViewModel->getReleaseById(releaseId);
    if (release == nullptr) {
        endResetModel();
        return;
    }

    auto countSeries = 0;
    auto series = torrent.series();
    if (series.indexOf("-") > -1) {
        auto parts = series.splitRef("-");
        auto start = parts[0].toInt();
        auto end = parts[1].toInt();

        countSeries = start == 1 && end == 1 ? 1 : end;
    } else {
        countSeries = series.toInt();
    }

    auto releaseVideos = m_releaseViewModel->getReleaseVideos(releaseId);
    auto videos = fillVideosFrom(releaseVideos, releaseId, release->poster());

    for (auto i = 0; i < countSeries; i++) {
        auto videoModel = new OnlineVideoModel();
        auto downloadedPath = torrentStream->getDownloadedPath(AnilibriaImagesPath + torrent.url(), i);
        QString url = "";
        if (downloadedPath.isEmpty()) {
            url = "http://localhost:" + QString::number(port) + "/online?id=" + QString::number(releaseId) + "&index=" + QString::number(i) + "&path=" + AnilibriaImagesPath + torrent.url();
        } else {
            url = downloadedPath;
        }
        videoModel->setFullHd(url);
        videoModel->setHd(url);
        videoModel->setSd(url);
        videoModel->setOrder(i);
        videoModel->setReleaseId(releaseId);
        videoModel->setReleasePoster(poster);
        videoModel->setIsGroup(false);
        videoModel->setTitle("Файл " + QString::number(i + 1));
        if (i < videos.count()) {
            videoModel->setOpeningEndSeconds(videos.value(i)->openingEndSeconds());
            videoModel->setOpeningStartSeconds(videos.value(i)->openingStartSeconds());
        }
        m_videos->append(videoModel);
    }

    endResetModel();
}

void OnlinePlayerVideoList::setVideosFromSingleList(int releaseId, const QString& poster) noexcept
{
    beginResetModel();

    auto apiVideos = m_releaseViewModel->getReleaseVideos(releaseId);
    auto release = m_releaseViewModel->getReleaseById(releaseId);
    auto releasePoster = release != nullptr ? release->poster() : "";

    m_videos->clear();

    m_videos->append(fillVideosFrom(apiVideos, releaseId, releasePoster));

    endResetModel();
}

inline void swap(QJsonValueRef v1, QJsonValueRef v2)
{
    QJsonValue temp(v1);
    v1 = QJsonValue(v2);
    v2 = temp;
}

void OnlinePlayerVideoList::setVideosFromCinemahall(QList<FullReleaseModel*>&& releases) noexcept
{
    if (releases.count() == 0) return;

    beginResetModel();

    m_videos->clear();

    foreach (auto release, releases) {
        auto videos = m_releaseViewModel->getReleaseVideos(release->id());

        std::sort(videos.begin(), videos.end(), [](const ReleaseOnlineVideoModel* left, const ReleaseOnlineVideoModel* right) {
            return left->order() < right->order();
        });

        auto groupModel = new OnlineVideoModel();
        groupModel->setTitle(release->title());
        groupModel->setIsGroup(true);
        m_videos->append(groupModel);

        auto releaseVideos = fillVideosFrom(videos, release->id(), release->poster());
        foreach (auto releaseVideo, releaseVideos) {
            m_videos->append(releaseVideo);
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

    m_openingStart = (*newSelected)->openingStartSeconds();
    m_openingEnd = (*newSelected)->openingEndSeconds();

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

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    return result.toSet().toList();
#else
    QSet<int> set(result.cbegin(), result.cend());

    return set.values();
#endif
}

void OnlinePlayerVideoList::setup(ReleasesViewModel* releaseViewModel)
{
    m_releaseViewModel = releaseViewModel;
}

void OnlinePlayerVideoList::refreshSingleVideo(int releaseId, int videoId) noexcept
{
    auto iterator = std::find_if(
        m_videos->begin(),
        m_videos->end(),
        [releaseId, videoId](OnlineVideoModel * video) {
            return video->releaseId() == releaseId && video->order() == videoId;
        }
    );
    if (iterator == m_videos->end()) return;

    auto videoIndex = m_videos->indexOf(*iterator);
    emit dataChanged(index(videoIndex), index(videoIndex));
}

bool OnlinePlayerVideoList::isPositionInOpening(int position) const noexcept
{
    if (m_openingStart == -1) return false;

    return position >= m_openingStart && position < m_openingEnd;
}

QVector<OnlineVideoModel*> OnlinePlayerVideoList::fillVideosFrom(const QList<ReleaseOnlineVideoModel *>& videos, int releaseId, const QString& poster)
{
    QVector<OnlineVideoModel*> result;

    foreach (auto video, videos) {
        auto videoModel = new OnlineVideoModel();
        videoModel->setReleaseId(releaseId);
        videoModel->setDescription(video->description());
        videoModel->setFullHd(video->fullhd());
        videoModel->setHd(video->hd());
        videoModel->setSd(video->sd());
        videoModel->setOrder(video->order());
        videoModel->setTitle(video->title());

        videoModel->setOpeningStartSeconds(video->openingStartSeconds());
        videoModel->setOpeningEndSeconds(video->openingEndSeconds());

        videoModel->setEndingStartSeconds(video->endingStartSeconds());
        videoModel->setEndingEndSeconds(video->endingEndSeconds());

        videoModel->setVideoPoster(video->videoPoster());
        videoModel->setReleasePoster(poster);
        videoModel->setIsGroup(false);
        result.append(videoModel);
    }

    std::sort(
        result.begin(),
        result.end(),
        [](const OnlineVideoModel* first, const OnlineVideoModel* second) {
            return first->id() < second->id();
        }
    );

    QVectorIterator<OnlineVideoModel*> iterator(result);
    int index = -1;
    while (iterator.hasNext()) {
        auto item = iterator.next();
        index += 1;
        item->setOrder(index);
    }

    return result;
}
