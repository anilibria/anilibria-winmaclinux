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

#include "onlineplayerviewmodel.h"
#include <math.h>
#include <QFile>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonArray>

OnlinePlayerViewModel::OnlinePlayerViewModel(QObject *parent) : QObject(parent),
    m_isFullScreen(false),
    m_playbackRate(1),
    m_isBuffering(false),
    m_videoQuality("sd"),
    m_isCinemahall(false),
    m_displayVideoPosition("00:00:00"),
    m_displayEndVideoPosition("00:00:00"),
    m_jumpMinutes(new QList<int>()),
    m_jumpSeconds(new QList<int>()),
    m_videoSource(""),
    m_releasePoster(""),
    m_isFullHdAllowed(false),
    m_selectedVideo(0),
    m_positionIterator(0),
    m_lastMovedPosition(0),
    m_restorePosition(0),
    m_lastMouseYPosition(0),
    m_selectedRelease(-1),
    m_ports(new QList<int>()),
    m_remotePlayer(new RemotePlayer(parent)),
    m_videoSourceChangedCommand("videosourcechanged"),
    m_videoPlaybackRateCommand("playbackratechanged"),
    m_videoPositionChangedCommand("positionchanged"),
    m_videoVolumeChangedCommand("volumechanged"),
    m_videoPlaybackCommand("playbackchanged"),
    m_seenModels(new QHash<int, SeenModel*>()),
    m_navigateReleaseId(0),
    m_customPlaylistPosition(-1),
    m_navigateVideos(""),
    m_navigatePoster("")
{
    createIfNotExistsFile(getSeensCachePath(), "[]");

    loadSeens();

    m_jumpMinutes->append(0);
    m_jumpMinutes->append(1);
    m_jumpMinutes->append(2);

    m_jumpSeconds->append(0);
    m_jumpSeconds->append(5);
    m_jumpSeconds->append(10);
    m_jumpSeconds->append(15);
    m_jumpSeconds->append(20);
    m_jumpSeconds->append(25);
    m_jumpSeconds->append(30);

    m_ports->append(12345);
    m_ports->append(34560);
    m_ports->append(52354);
    m_ports->append(67289);

    connect(m_remotePlayer, &RemotePlayer::receiveCommand, this, &OnlinePlayerViewModel::receiveCommand);
}

void OnlinePlayerViewModel::setIsFullScreen(bool isFullScreen) noexcept
{
    if (m_isFullScreen == isFullScreen) return;

    m_isFullScreen = isFullScreen;
    emit isFullScreenChanged();
}

void OnlinePlayerViewModel::setPlaybackRate(qreal playbackRate) noexcept
{
    if (m_playbackRate == playbackRate) return;

    m_playbackRate = playbackRate;
    emit playbackRateChanged();

    m_remotePlayer->broadcastCommand(m_videoPlaybackRateCommand, QString::number(playbackRate));
}

void OnlinePlayerViewModel::setIsBuffering(bool isBuffering) noexcept
{
    if (m_isBuffering == isBuffering) return;

    m_isBuffering = isBuffering;
    emit isBufferingChanged();
}

void OnlinePlayerViewModel::setVideoQuality(const QString &videoQuality) noexcept
{
    if (m_videoQuality == videoQuality) return;

    m_videoQuality = videoQuality;
    emit videoQualityChanged();
}

void OnlinePlayerViewModel::setIsCinemahall(bool isCinemahall) noexcept
{
    if (m_isCinemahall == isCinemahall) return;

    m_isCinemahall = isCinemahall;
    emit isCinemahallChanged();
}

void OnlinePlayerViewModel::setDisplayVideoPosition(QString displayVideoPosition) noexcept
{
    if (m_displayVideoPosition == displayVideoPosition) return;

    m_displayVideoPosition = displayVideoPosition;
    emit displayVideoPositionChanged();
}

void OnlinePlayerViewModel::setDisplayEndVideoPosition(const QString &displayEndVideoPosition)
{
    if (m_displayEndVideoPosition == displayEndVideoPosition) return;

    m_displayEndVideoPosition = displayEndVideoPosition;
    emit displayEndVideoPositionChanged();
}

void OnlinePlayerViewModel::setVideoSource(const QString &videoSource)
{
    if (m_videoSource == videoSource) return;

    m_videoSource = videoSource;
    emit videoSourceChanged();

    m_remotePlayer->broadcastCommand(m_videoSourceChangedCommand, videoSource);
}

void OnlinePlayerViewModel::setReleasePoster(const QString &releasePoster) noexcept
{
    if (m_releasePoster == releasePoster) return;

    m_releasePoster = releasePoster;
    emit releasePosterChanged();
}

void OnlinePlayerViewModel::setIsFullHdAllowed(bool isFullHdAllowed) noexcept
{
    if (m_isFullHdAllowed == isFullHdAllowed) return;

    m_isFullHdAllowed = isFullHdAllowed;
    emit isFullHdAllowedChanged();
}

void OnlinePlayerViewModel::setSelectedVideo(int selectedVideo) noexcept
{
    if (m_selectedVideo == selectedVideo) return;

    m_selectedVideo = selectedVideo;
    emit selectedVideoChanged();
}

void OnlinePlayerViewModel::setPositionIterator(int positionIterator) noexcept
{
    if (m_positionIterator == positionIterator) return;

    m_positionIterator = positionIterator;
    emit positionIteratorChanged();
}

void OnlinePlayerViewModel::setLastMovedPosition(int lastMovedPosition) noexcept
{
    if (m_lastMovedPosition == lastMovedPosition) return;

    m_lastMovedPosition = lastMovedPosition;
    emit lastMovedPositionChanged();
}

void OnlinePlayerViewModel::setRestorePosition(int restorePosition) noexcept
{
    if (m_restorePosition == restorePosition) return;

    m_restorePosition = restorePosition;
    emit restorePositionChanged();
}

void OnlinePlayerViewModel::setLastMouseYPosition(int lastMouseYPosition) noexcept
{
    if (m_lastMouseYPosition == lastMouseYPosition) return;

    m_lastMouseYPosition = lastMouseYPosition;
    emit lastMouseYPositionChanged();
}

void OnlinePlayerViewModel::setSelectedRelease(int selectedRelease) noexcept
{
    if (m_selectedRelease == selectedRelease) return;

    m_selectedRelease = selectedRelease;
    emit selectedReleaseChanged();
}

void OnlinePlayerViewModel::setSendPlaybackToRemoteSwitch(bool sendPlaybackToRemoteSwitch)
{
    if (m_sendPlaybackToRemoteSwitch == sendPlaybackToRemoteSwitch) return;

    m_sendPlaybackToRemoteSwitch = sendPlaybackToRemoteSwitch;
    emit sendPlaybackToRemoteSwitchChanged();
}

void OnlinePlayerViewModel::setVolumeSlider(int volumeSlider) noexcept
{
    if (m_volumeSlider == volumeSlider) return;

    m_volumeSlider = volumeSlider;
    emit volumeSliderChanged();
}

void OnlinePlayerViewModel::setPlayerPlaybackState(int playerPlaybackState) noexcept
{
    if (m_playerPlaybackState == playerPlaybackState) return;

    m_playerPlaybackState = playerPlaybackState;
    emit playerPlaybackStateChanged();
}

void OnlinePlayerViewModel::setNavigateReleaseId(int navigateReleaseId) noexcept
{
    if (m_navigateReleaseId == navigateReleaseId) return;

    m_navigateReleaseId = navigateReleaseId;
    emit navigateReleaseIdChanged();
}

void OnlinePlayerViewModel::setCustomPlaylistPosition(int customPlaylistPosition) noexcept
{
    if (m_customPlaylistPosition == customPlaylistPosition) return;

    m_customPlaylistPosition = customPlaylistPosition;
    emit customPlaylistPositionChanged();
}

void OnlinePlayerViewModel::setNavigateVideos(const QString &navigateVideos)
{
    if (m_navigateVideos == navigateVideos) return;

    m_navigateVideos = navigateVideos;
    emit navigateVideosChanged();
}

void OnlinePlayerViewModel::setNavigatePoster(const QString &navigatePoster) noexcept
{
    if (m_navigatePoster == navigatePoster) return;

    m_navigatePoster = navigatePoster;
    emit navigatePosterChanged();
}

void OnlinePlayerViewModel::toggleFullScreen()
{
    setIsFullScreen(!m_isFullScreen);
}

void OnlinePlayerViewModel::changeVideoPosition(int duration, int position) noexcept
{
    QString start = getDisplayTimeFromSeconds(position / 1000);
    QString end = getDisplayTimeFromSeconds(duration / 1000);

    setDisplayVideoPosition(start + " из " + end);

    setDisplayEndVideoPosition(getDisplayTimeFromSeconds((duration - position) / 1000));
}

QString OnlinePlayerViewModel::checkExistingVideoQuality(int index)
{
    auto video = m_videos->getVideoAtIndex(index);
    if (video == nullptr) return "";

    return getVideoFromQuality(video);
}

void OnlinePlayerViewModel::nextVideo()
{
    setRestorePosition(0);

    OnlineVideoModel* video;

    if (m_isCinemahall) {
        auto nextVideo = nextNotSeenVideo();
        if (nextVideo == nullptr) return;

        setSelectedVideo(nextVideo->order());
        video = nextVideo;
    } else {
        if (m_selectedVideo == m_videos->getReleaseVideosCount(m_selectedRelease)) return;

        setSelectedVideo(m_selectedVideo + 1);

        video = m_videos->getVideoAtIndex(m_selectedVideo);
    }

    setIsFullHdAllowed(!video->fullhd().isEmpty());
    setReleasePoster(video->releasePoster());
    setSelectedRelease(video->releaseId());

    setVideoSource(getVideoFromQuality(video));

    if (!m_isCinemahall) emit needScrollSeriaPosition();
}

void OnlinePlayerViewModel::previousVideo()
{
    if (m_selectedVideo == 0) return;
    setRestorePosition(0);

    OnlineVideoModel* video;

    if (m_isCinemahall) {
        auto previousVideo = previousNotSeenVideo();
        if (previousVideo) {
            setSelectedVideo(previousVideo->order());
            video = previousVideo;
        } else {
            return;
        }

    } else {
        setSelectedVideo(m_selectedVideo - 1);
        video = m_videos->getVideoAtIndex(m_selectedVideo);
    }

    setIsFullHdAllowed(!video->fullhd().isEmpty());
    setReleasePoster(video->releasePoster());
    setSelectedRelease(video->releaseId());

    setVideoSource(getVideoFromQuality(video));

    if (!m_isCinemahall) emit needScrollSeriaPosition();
}

QString OnlinePlayerViewModel::getVideoSeen(int id)
{
    if (m_seenModels->contains(id)) {
        auto seenModel = m_seenModels->value(id);
        QJsonObject object;
        seenModel->writeToJson(object);

        QJsonDocument seenDocument(object);
        QString seenJson(seenDocument.toJson());
        return seenJson;
    } else {
        return "{}";
    }
}

static bool compareSeenTimeStampDescending(const SeenModel* first, const SeenModel* second)
{
    return first->timestamp() > second->timestamp();
}

int OnlinePlayerViewModel::getLastVideoSeen()
{
    if (m_seenModels->count() == 0) return 0;


    auto models = m_seenModels->values();

    std::sort(models.begin(), models.end(), compareSeenTimeStampDescending);
    return models.first()->id();
}

void OnlinePlayerViewModel::setVideoSeens(int id, int videoId, double videoPosition)
{
    QDateTime now = QDateTime::currentDateTime();
    auto timestamp = now.toTime_t();
    if (!m_seenModels->contains(id)) {
        SeenModel* seenModel = new SeenModel();
        seenModel->setId(id);
        seenModel->setVideoId(videoId);
        seenModel->setVideoPosition(videoPosition);
        seenModel->setTimestamp(static_cast<int>(timestamp));

        m_seenModels->insert(id, seenModel);
    } else {
        auto existingSeenModel = m_seenModels->value(id);
        existingSeenModel->setVideoId(videoId);
        existingSeenModel->setVideoPosition(videoPosition);
        existingSeenModel->setTimestamp(static_cast<int>(timestamp));
    }

    saveVideoSeens();
}

void OnlinePlayerViewModel::setupForSingleRelease()
{
    setIsCinemahall(false);

    m_videos->setVideosFromSingleList(m_navigateVideos, m_navigateReleaseId, m_navigatePoster);

    setReleasePoster(m_navigatePoster);
    setSelectedRelease(m_navigateReleaseId);

    int videoIndex = 0;
    if (m_seenModels->contains(m_navigateReleaseId)) {
        auto model = m_seenModels->value(m_navigateReleaseId);
        videoIndex = model->videoId();
    }

    if (m_customPlaylistPosition > -1) videoIndex = m_customPlaylistPosition;

    auto firstVideo = m_videos->getVideoAtIndex(videoIndex);

    //refreshSeenMarks();

    setSelectedVideo(firstVideo->order());
    setIsFullHdAllowed(!firstVideo->fullhd().isEmpty());
    setVideoSource(getVideoFromQuality(firstVideo));

    emit playInPlayer();
    emit saveToWatchHistory(m_navigateReleaseId);
    emit needScrollSeriaPosition();
}

void OnlinePlayerViewModel::saveVideoSeens()
{
    QJsonArray array;

    QHashIterator<int, SeenModel*> iterator(*m_seenModels);
    while (iterator.hasNext()) {
        iterator.next();

        QJsonObject object;
        iterator.value()->writeToJson(object);
        array.append(object);
    }

    QJsonDocument seenDocument(array);
    QString seenJson(seenDocument.toJson());

    QFile seenFile(getSeensCachePath());
    if (!seenFile.open(QFile::WriteOnly | QFile::Text)) {
        //TODO: handle this situation
    }
    seenFile.write(seenJson.toUtf8());
    seenFile.close();
}

QString OnlinePlayerViewModel::getZeroBasedDigit(int digit)
{
    if (digit < 10) return "0" + QString::number(digit);
    return QString::number(digit);
}

QString OnlinePlayerViewModel::getDisplayTimeFromSeconds(int seconds)
{
    int days = floor(seconds / (3600 * 24));
    seconds -= days * 3600 * 24;
    int hours = floor(seconds / 3600);
    seconds -= hours * 3600;
    int minutes = floor(seconds / 60);
    seconds  -= minutes * 60;

    QString result;
    result.append(getZeroBasedDigit(hours));
    result.append(":");
    result.append(getZeroBasedDigit(minutes));
    result.append(":");
    result.append(getZeroBasedDigit(seconds));
    return result;
}

QString OnlinePlayerViewModel::getVideoFromQuality(OnlineVideoModel *video)
{
    if (m_videoQuality == "sd" && !video->sd().isEmpty()) {
        return video->sd();
    } else if (m_videoQuality == "hd" && !video->hd().isEmpty()) {
        return video->hd();
    } else if (m_videoQuality == "fullhd" && !video->fullhd().isEmpty()) {
        return video->fullhd();
    }

    if (!video->sd().isEmpty()) {
        setVideoQuality("sd");
        return video->sd();
    }
    if (!video->hd().isEmpty()) {
        setVideoQuality("hd");
        return video->hd();
    }
    if (!video->fullhd().isEmpty()) {
        setVideoQuality("fullhd");
        return video->fullhd();
    }

    return "";
}

OnlineVideoModel* OnlinePlayerViewModel::nextNotSeenVideo()
{
    auto selectedRelease = m_selectedRelease;
    auto selectedVideo = m_selectedVideo;
    return m_videos->getFirstReleaseWithPredicate(
        [selectedRelease, selectedVideo](OnlineVideoModel* video) {
            bool beforeCurrent = true;

            if (video->releaseId() == selectedRelease && video->order() <= selectedVideo) {
                beforeCurrent = false;
                return false;
            }
            if (beforeCurrent) return false;
            if (video->isGroup()) return false;

            //TODO: add seen marks
            /*if (!(releaseVideo.releaseId in _page.seenMarks && releaseVideo.order in _page.seenMarks[releaseVideo.releaseId])) {
                return releaseVideo;
            }*/

            return true;
        }
    );
}

OnlineVideoModel *OnlinePlayerViewModel::previousNotSeenVideo()
{
    auto selectedRelease = m_selectedRelease;
    auto selectedVideo = m_selectedVideo;

    return m_videos->getFirstReleaseWithPredicate(
        [selectedRelease, selectedVideo](OnlineVideoModel* video) {
            if (video->isGroup()) return false;

            if (video->releaseId() == selectedRelease && video->order() == selectedVideo) return false;

            //TODO: add seen marks
            /*if (!(releaseVideo.releaseId in _page.seenMarks && releaseVideo.order in _page.seenMarks[releaseVideo.releaseId])) lastNotSeenVideo = releaseVideo;
            }*/

            return true;
        }
    );
}

void OnlinePlayerViewModel::receiveCommand(const unsigned int id, const QString &command, const QString &argument)
{
    qDebug() << argument;

    if (command == "getcurrentvideosource"){
         m_remotePlayer->sendCommandToUser(id, m_videoSourceChangedCommand, m_videoSource);
    }
    if (command == "getcurrentvideoposition"){
        //TODO: added position and duration from player
         m_remotePlayer->sendCommandToUser(id, m_videoPositionChangedCommand, ""/*player.position.toString() + `/` + player.duration.toString()*/);
    }
    if (command == "getcurrentvolume"){
         if (m_sendPlaybackToRemoteSwitch) m_remotePlayer->sendCommandToUser(id, m_videoVolumeChangedCommand, QString::number(m_volumeSlider));
    }
    if (command == "getcurrentplaybackrate"){
         m_remotePlayer->sendCommandToUser(id, m_videoPlaybackRateCommand, QString::number(m_playbackRate));
    }
    if (command == "getcurrentplayback"){
        qDebug() << "m_playerPlaybackState: " << m_playerPlaybackState;
        if (m_playerPlaybackState == 3 && m_sendPlaybackToRemoteSwitch) m_remotePlayer->sendCommandToUser(id, m_videoPlaybackCommand, "pause");
    }
}

void OnlinePlayerViewModel::loadSeens()
{
    QFile seenFile(getSeensCachePath());
    if (!seenFile.open(QFile::ReadOnly | QFile::Text)) {
        //TODO: handle this situation
    }
    auto seenJson = seenFile.readAll();
    seenFile.close();

    auto document = QJsonDocument::fromJson(seenJson);
    auto jsonSeens = document.array();

    foreach (auto item, jsonSeens) {
        SeenModel* seenModel = new SeenModel();
        seenModel->readFromJson(item);
        if (!m_seenModels->contains(seenModel->id())) {
            m_seenModels->insert(seenModel->id(), seenModel);
        }
    }
}

QString OnlinePlayerViewModel::getSeensCachePath()
{
    if (IsPortable) {
        return QDir::currentPath() + "/seen.cache";
    } else {
        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/seen.cache";
    }
}

void OnlinePlayerViewModel::createIfNotExistsFile(QString path, QString defaultContent)
{
    if (!QFile::exists(path)) {
        QFile file(path);
        file.open(QFile::WriteOnly | QFile::Text);
        file.write(defaultContent.toUtf8());
        file.close();
    }
}