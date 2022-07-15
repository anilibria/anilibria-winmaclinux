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
#include <QTime>
#include "releaseonlineserieslistmodel.h"
#include "../../globalconstants.h"

ReleaseOnlineSeriesListModel::ReleaseOnlineSeriesListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

int ReleaseOnlineSeriesListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_items.size();
}

QVariant ReleaseOnlineSeriesListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto currentIndex = index.row();
    if (m_releaseId == 0) return QVariant();

    auto onlineVideo = m_items.at(currentIndex);
    auto seens = m_onlinePlayer->getSeenVideoPosition(m_releaseId);
    auto videoId = std::get<0>(seens);
    auto videoPosition = std::get<1>(seens);
    auto currentVideoId = onlineVideo->id() - 1;
    auto isEmptyPoster = onlineVideo->videoPoster().isEmpty();

    switch (role) {
        case IdRole: {
            return QVariant(onlineVideo->id());
        }
        case IndexRole: {
            return QVariant(currentVideoId);
        }
        case PosterRole: {
            if (isEmptyPoster) {
                return QVariant("qrc:///Assets/Icons/broken.svg");
            } else {
                return QVariant(AnilibriaImagesPath + onlineVideo->videoPoster());
            }
        }
        case IsEmptyPosterRole: {
            return QVariant(isEmptyPoster);
        }
        case IsSeensRole: {
            return QVariant(m_releases->getSeriaSeenMark(m_releaseId, currentVideoId));
        }
        case IsCurrentVideoRole: {
            return QVariant(videoId == currentVideoId);
        }
        case CurrentTimeVideoRole: {
            return videoId == currentVideoId && videoPosition > 0 ? QVariant("Остановились на " + getDisplayTime(videoPosition)) : QVariant("");
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ReleaseOnlineSeriesListModel::roleNames() const
{
    return {
        {
            IdRole,
            "id"
        },
        {
            IndexRole,
            "index"
        },
        {
            PosterRole,
            "poster"
        },
        {
            IsSeensRole,
            "isSeens"
        },
        {
            IsCurrentVideoRole,
            "isCurrentVideo"
        },
        {
            CurrentTimeVideoRole,
            "currentTimeVideo"
        },
        {
            IsEmptyPosterRole,
            "isEmptyPoster"
        }
    };
}

void ReleaseOnlineSeriesListModel::setReleaseId(int releaseId) noexcept
{
    if (m_releaseId == releaseId) return;

    m_releaseId = releaseId;
    emit releaseIdChanged();
    refresh();
}

void ReleaseOnlineSeriesListModel::setReleases(const ReleasesViewModel *releases) noexcept
{
    if (m_releases == releases) return;

    m_releases = const_cast<ReleasesViewModel *>(releases);
    emit releasesChanged();
}

void ReleaseOnlineSeriesListModel::setOnlinePlayer(const OnlinePlayerViewModel *onlinePlayer) noexcept
{
    if (m_onlinePlayer == onlinePlayer) return;

    m_onlinePlayer = const_cast<OnlinePlayerViewModel *>(onlinePlayer);
    emit onlinePlayerChanged();
}

void ReleaseOnlineSeriesListModel::refresh()
{
    beginResetModel();

    m_items.clear();

    if (m_releaseId == 0) {
        endResetModel();
        return;
    }

    auto videos = m_releases->getReleaseVideos(m_releaseId);
    auto document = QJsonDocument::fromJson(videos.toUtf8());
    auto videosArray = document.array();

    foreach (auto video, videosArray) {
        auto videoModel = new OnlineVideoModel();
        videoModel->readFromApiModel(video.toObject());
        m_items.append(videoModel);
    }

    std::sort(
        m_items.begin(),
        m_items.end(),
        [](const OnlineVideoModel* first, const OnlineVideoModel* second) {
            return first->id() < second->id();
        }
    );

    endResetModel();

    emit isEmptyChanged();
}

QString ReleaseOnlineSeriesListModel::getDisplayTime(int milliseconds) const
{
    auto time = QTime(0, 0, 0).addMSecs(milliseconds);
    auto second = time.second();
    auto minutes = time.minute();
    auto hours = time.hour();

    if (second == 0) return "";

    if (hours > 0) return getZeroValue(hours) + ":" + getZeroValue(minutes) + ":" + getZeroValue(second);

    if (minutes == 0) return getZeroValue(second) + " секунд";

    return getZeroValue(minutes) + " минут " + getZeroValue(second) + " секунд";
}

QString ReleaseOnlineSeriesListModel::getZeroValue(int value) const
{
    return (value < 10 ? "0" : "") + QString::number(value);
}
