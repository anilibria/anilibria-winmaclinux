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

#ifndef ONLINEPLAYERVIEWMODEL_H
#define ONLINEPLAYERVIEWMODEL_H

#include <QObject>
#include <QHash>
#include <QList>
#include "../ListModels/onlineplayervideolist.h"
#include "../RemotePlayer/remoteplayer.h"
#include "../Models/seenmodel.h"
#include "../../globalconstants.h"
#include "../ViewModels/releasesviewmodel.h"

class OnlinePlayerViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isFullScreen READ isFullScreen WRITE setIsFullScreen NOTIFY isFullScreenChanged)
    Q_PROPERTY(qreal playbackRate READ playbackRate WRITE setPlaybackRate NOTIFY playbackRateChanged)
    Q_PROPERTY(bool isBuffering READ isBuffering WRITE setIsBuffering NOTIFY isBufferingChanged)
    Q_PROPERTY(QString videoQuality READ videoQuality WRITE setVideoQuality NOTIFY videoQualityChanged)
    Q_PROPERTY(bool isCinemahall READ isCinemahall WRITE setIsCinemahall NOTIFY isCinemahallChanged)
    Q_PROPERTY(QString displayVideoPosition READ displayVideoPosition NOTIFY displayVideoPositionChanged)
    Q_PROPERTY(QString displayEndVideoPosition READ displayEndVideoPosition WRITE setDisplayEndVideoPosition NOTIFY displayEndVideoPositionChanged)
    Q_PROPERTY(QList<int> jumpMinutes READ jumpMinutes NOTIFY jumpMinutesChanged)
    Q_PROPERTY(QList<int> jumpSeconds READ jumpSeconds NOTIFY jumpSecondsChanged)
    Q_PROPERTY(OnlinePlayerVideoList* videos READ videos NOTIFY videosChanged)
    Q_PROPERTY(QString videoSource READ videoSource WRITE setVideoSource NOTIFY videoSourceChanged)
    Q_PROPERTY(QString releasePoster READ releasePoster WRITE setReleasePoster NOTIFY releasePosterChanged)
    Q_PROPERTY(bool isFullHdAllowed READ isFullHdAllowed WRITE setIsFullHdAllowed NOTIFY isFullHdAllowedChanged)
    Q_PROPERTY(int selectedVideo READ selectedVideo WRITE setSelectedVideo NOTIFY selectedVideoChanged)
    Q_PROPERTY(int positionIterator READ positionIterator WRITE setPositionIterator NOTIFY positionIteratorChanged)
    Q_PROPERTY(int lastMovedPosition READ lastMovedPosition WRITE setLastMovedPosition NOTIFY lastMovedPositionChanged)
    Q_PROPERTY(int restorePosition READ restorePosition WRITE setRestorePosition NOTIFY restorePositionChanged)
    Q_PROPERTY(int lastMouseYPosition READ lastMouseYPosition WRITE setLastMouseYPosition NOTIFY lastMouseYPositionChanged)
    Q_PROPERTY(int selectedRelease READ selectedRelease WRITE setSelectedRelease NOTIFY selectedReleaseChanged)
    Q_PROPERTY(QList<int> ports READ ports NOTIFY portsChanged)
    Q_PROPERTY(RemotePlayer* remotePlayer READ remotePlayer NOTIFY remotePlayerChanged)
    Q_PROPERTY(bool sendPlaybackToRemoteSwitch READ sendPlaybackToRemoteSwitch WRITE setSendPlaybackToRemoteSwitch NOTIFY sendPlaybackToRemoteSwitchChanged)
    Q_PROPERTY(int volumeSlider READ volumeSlider WRITE setVolumeSlider NOTIFY volumeSliderChanged)
    Q_PROPERTY(int playerPlaybackState READ playerPlaybackState WRITE setPlayerPlaybackState NOTIFY playerPlaybackStateChanged)
    Q_PROPERTY(int navigateReleaseId READ navigateReleaseId WRITE setNavigateReleaseId NOTIFY navigateReleaseIdChanged)
    Q_PROPERTY(int customPlaylistPosition READ customPlaylistPosition WRITE setCustomPlaylistPosition NOTIFY customPlaylistPositionChanged)
    Q_PROPERTY(QString navigateVideos READ navigateVideos WRITE setNavigateVideos NOTIFY navigateVideosChanged)
    Q_PROPERTY(QString navigatePoster READ navigatePoster WRITE setNavigatePoster NOTIFY navigatePosterChanged)
    Q_PROPERTY(int videoPosition READ videoPosition WRITE setVideoPosition NOTIFY videoPositionChanged)
    Q_PROPERTY(int videoDuration READ videoDuration WRITE setVideoDuration NOTIFY videoDurationChanged)
    Q_PROPERTY(bool sendVolumeToRemote READ sendVolumeToRemote WRITE setSendVolumeToRemote NOTIFY sendVolumeToRemoteChanged)
    Q_PROPERTY(bool isMultipleRelease READ isMultipleRelease WRITE setIsMultipleRelease NOTIFY isMultipleReleaseChanged)
    Q_PROPERTY(bool isFromNavigated READ isFromNavigated WRITE setIsFromNavigated NOTIFY isFromNavigatedChanged)
    Q_PROPERTY(ReleasesViewModel* releasesViewModel READ releasesViewModel WRITE setReleasesViewModel NOTIFY releasesViewModelChanged)
    Q_PROPERTY(bool showNextPosterRelease READ showNextPosterRelease WRITE setShowNextPosterRelease NOTIFY showNextPosterReleaseChanged)
    Q_PROPERTY(QString nextReleasePoster READ nextReleasePoster WRITE setNextReleasePoster NOTIFY nextReleasePosterChanged)
    Q_PROPERTY(bool seenMarkedAtEnd READ seenMarkedAtEnd WRITE setSeenMarkedAtEnd NOTIFY seenMarkedAtEndChanged)
    Q_PROPERTY(bool displaySkipOpening READ displaySkipOpening WRITE setDisplaySkipOpening NOTIFY displaySkipOpeningChanged)

private:
    bool m_isFullScreen;
    qreal m_playbackRate;
    bool m_isBuffering;
    QString m_videoQuality;
    bool m_isCinemahall;
    QString m_displayVideoPosition;
    QString m_displayEndVideoPosition;
    QList<int>* m_jumpMinutes;
    QList<int>* m_jumpSeconds;
    OnlinePlayerVideoList* m_videos;
    QString m_videoSource;
    QString m_releasePoster;
    bool m_isFullHdAllowed;
    int m_selectedVideo;
    int m_positionIterator;
    int m_lastMovedPosition;
    int m_restorePosition;
    int m_lastMouseYPosition;
    int m_selectedRelease;
    QList<int>* m_ports;
    RemotePlayer* m_remotePlayer;
    QString m_videoSourceChangedCommand;
    QString m_videoPlaybackRateCommand;
    QString m_videoPositionChangedCommand;
    QString m_videoVolumeChangedCommand;
    QString m_videoPlaybackCommand;
    bool m_sendPlaybackToRemoteSwitch;
    int m_volumeSlider;
    int m_playerPlaybackState;
    QHash<int, SeenModel*>* m_seenModels;
    int m_navigateReleaseId;
    int m_customPlaylistPosition;
    QString m_navigateVideos;
    QString m_navigatePoster;
    int m_videoPosition;
    int m_videoDuration;
    bool m_sendVolumeToRemote;
    bool m_isMultipleRelease;
    bool m_isFromNavigated;
    int m_watchedTimes { 0 };
    ReleasesViewModel* m_releasesViewModel;
    bool m_showNextPosterRelease { false };
    QString m_nextReleasePoster { "" };
    bool m_seenMarkedAtEnd { false };
    bool m_displaySkipOpening { false };

public:
    explicit OnlinePlayerViewModel(QObject *parent = nullptr);

    bool isFullScreen() const { return m_isFullScreen; }
    void setIsFullScreen(bool isFullScreen) noexcept;

    qreal playbackRate() const { return m_playbackRate; }
    void setPlaybackRate(qreal playbackRate) noexcept;

    bool isBuffering() const { return m_isBuffering; }
    void setIsBuffering(bool isBuffering) noexcept;

    QString videoQuality() const { return m_videoQuality; }
    void setVideoQuality(const QString& videoQuality) noexcept;

    bool isCinemahall() const { return m_isCinemahall; }
    void setIsCinemahall(bool isCinemahall) noexcept;

    QString displayVideoPosition() const { return m_displayVideoPosition; }
    void setDisplayVideoPosition(QString displayVideoPosition) noexcept;

    QString displayEndVideoPosition() const { return m_displayEndVideoPosition; }
    void setDisplayEndVideoPosition(const QString& displayEndVideoPosition);

    QList<int> jumpMinutes() const { return *m_jumpMinutes; }

    QList<int> jumpSeconds() const { return *m_jumpSeconds; }

    OnlinePlayerVideoList* videos() const { return m_videos; }

    QString videoSource() const { return m_videoSource; }
    void setVideoSource(const QString& videoSource);

    QString releasePoster() const { return m_releasePoster; }
    void setReleasePoster(const QString& releasePoster) noexcept;

    bool isFullHdAllowed() const { return m_isFullHdAllowed; }
    void setIsFullHdAllowed(bool isFullHdAllowed) noexcept;

    int selectedVideo() const { return m_selectedVideo; }
    void setSelectedVideo(int selectedVideo) noexcept;

    int positionIterator() const { return m_positionIterator; }
    void setPositionIterator(int positionIterator) noexcept;

    int lastMovedPosition() const { return m_lastMovedPosition; }
    void setLastMovedPosition(int lastMovedPosition) noexcept;

    int restorePosition() const { return m_restorePosition; }
    void setRestorePosition(int restorePosition) noexcept;

    int lastMouseYPosition() const { return m_lastMouseYPosition; }
    void setLastMouseYPosition(int lastMouseYPosition) noexcept;

    int selectedRelease() const { return m_selectedRelease; }
    void setSelectedRelease(int selectedRelease) noexcept;

    QList<int> ports() const { return *m_ports; }

    RemotePlayer* remotePlayer() const { return m_remotePlayer; }

    bool sendPlaybackToRemoteSwitch() const { return m_sendPlaybackToRemoteSwitch; }
    void setSendPlaybackToRemoteSwitch(bool sendPlaybackToRemoteSwitch);

    int volumeSlider() const { return m_volumeSlider; }
    void setVolumeSlider(int volumeSlider) noexcept;

    int playerPlaybackState() const { return m_playerPlaybackState; }
    void setPlayerPlaybackState(int playerPlaybackState) noexcept;

    int navigateReleaseId() const { return m_navigateReleaseId; }
    void setNavigateReleaseId(int navigateReleaseId) noexcept;

    int customPlaylistPosition() const { return m_customPlaylistPosition; }
    void setCustomPlaylistPosition(int customPlaylistPosition) noexcept;

    QString navigateVideos() const { return m_navigateVideos; }
    void setNavigateVideos(const QString& navigateVideos);

    QString navigatePoster() const { return m_navigatePoster; }
    void setNavigatePoster(const QString& navigatePoster) noexcept;

    int videoPosition() const { return m_videoPosition; }
    void setVideoPosition(int position) noexcept;

    int videoDuration() const { return m_videoDuration; }
    void setVideoDuration(int videoDuration) noexcept;

    bool sendVolumeToRemote() const { return m_sendVolumeToRemote; }
    void setSendVolumeToRemote(bool sendVolumeToRemote) noexcept;

    bool isMultipleRelease() const { return m_isMultipleRelease; }
    void setIsMultipleRelease(bool isMultipleRelease) noexcept;

    bool isFromNavigated() const noexcept { return m_isFromNavigated; }
    void setIsFromNavigated(const bool& isFromNavigated) noexcept;

    ReleasesViewModel* releasesViewModel() const noexcept { return m_releasesViewModel; }
    void setReleasesViewModel(ReleasesViewModel* releasesViewModel) noexcept;

    bool showNextPosterRelease() const noexcept { return m_showNextPosterRelease; }
    void setShowNextPosterRelease(bool showNextPosterRelease) noexcept;

    QString nextReleasePoster() const noexcept { return m_nextReleasePoster; }
    void setNextReleasePoster(QString nextReleasePoster) noexcept;

    bool seenMarkedAtEnd() const noexcept { return m_seenMarkedAtEnd; }
    void setSeenMarkedAtEnd(bool seenMarkedAtEnd) noexcept;

    bool displaySkipOpening() const noexcept { return m_displaySkipOpening; }
    void setDisplaySkipOpening(bool displaySkipOpening) noexcept;

    std::tuple<int, int> getSeenVideoPosition(int releaseId);

    Q_INVOKABLE void toggleFullScreen();
    Q_INVOKABLE void changeVideoPosition(int duration, int position) noexcept;
    Q_INVOKABLE QString checkExistingVideoQuality(int index);    
    Q_INVOKABLE void nextVideo();
    Q_INVOKABLE void previousVideo();
    Q_INVOKABLE QString getVideoSeen(int id);
    Q_INVOKABLE int getCurrentVideoSeenVideoPosition();
    Q_INVOKABLE int getLastVideoSeen();
    Q_INVOKABLE void setVideoSeens(int id, int videoId, double videoPosition);
    Q_INVOKABLE void quickSetupForSingleRelease(int releaseId);
    Q_INVOKABLE void quickSetupForMultipleRelease(QList<int> releaseIds);
    Q_INVOKABLE void quickSetupForFavoritesCinemahall();
    Q_INVOKABLE void setupForSingleRelease();
    Q_INVOKABLE void setupForMultipleRelease();
    Q_INVOKABLE void setupForCinemahall();
    Q_INVOKABLE QString getReleasesSeenMarks(QList<int> ids);
    Q_INVOKABLE void selectVideo(int releaseId, int videoId);
    Q_INVOKABLE void changeVideoQuality(const QString& quality) noexcept;
    Q_INVOKABLE void setVideoSpeed(double speed) noexcept;
    Q_INVOKABLE void broadcastPlaybackState(const QString& state) noexcept;
    Q_INVOKABLE void broadcastVolume(int volume) noexcept;
    Q_INVOKABLE void broadcastVideoPosition(const QString& position) noexcept;
    Q_INVOKABLE QList<int> getReleaseIds() noexcept;
    Q_INVOKABLE int jumpInPlayer(int minutes, int seconds, bool direction) noexcept;
    Q_INVOKABLE bool isLastSeriaIsSingleRelease() const noexcept;
    Q_INVOKABLE void refreshSingleVideo(int releaseId, int videoId) noexcept;
    Q_INVOKABLE int skipOpening() noexcept;

private:
    void saveVideoSeens();
    QString getZeroBasedDigit(int digit);
    QString getDisplayTimeFromSeconds(int seconds);
    QString getVideoFromQuality(OnlineVideoModel* video);
    OnlineVideoModel* nextNotSeenVideo();
    OnlineVideoModel* previousNotSeenVideo();
    void receiveCommand(const unsigned int id, const QString& command, const QString& argument);
    void loadSeens();
    QString getSeensCachePath();
    void createIfNotExistsFile(QString path, QString defaultContent);
    void receiveRemotePort();
    void receiveCountUsers();

signals:
    void isFullScreenChanged();
    void playbackRateChanged();
    void isBufferingChanged();
    void videoQualityChanged();
    void isCinemahallChanged();
    void displayVideoPositionChanged();
    void displayEndVideoPositionChanged();
    void jumpMinutesChanged();
    void jumpSecondsChanged();
    void videosChanged();
    void videoSourceChanged();
    void releasePosterChanged();
    void isFullHdAllowedChanged();
    void selectedVideoChanged();
    void positionIteratorChanged();
    void lastMovedPositionChanged();
    void restorePositionChanged();
    void lastMouseYPositionChanged();
    void selectedReleaseChanged();
    void portsChanged();
    void remotePlayerChanged();
    void sendPlaybackToRemoteSwitchChanged();
    void volumeSliderChanged();
    void playerPlaybackStateChanged();
    void needScrollSeriaPosition();
    void navigateReleaseIdChanged();
    void customPlaylistPositionChanged();
    void navigateVideosChanged();
    void navigatePosterChanged();
    void playInPlayer();
    void saveToWatchHistory(int releaseId);
    void recalculateSeenCounts();
    void remotePlayerStartedChanged();
    void remotePlayerCountUsersChanged();
    void refreshSeenMarks();
    void stopInPlayer();
    void videoPositionChanged();
    void videoDurationChanged();
    void sendVolumeToRemoteChanged();
    void isMultipleReleaseChanged();
    void isFromNavigatedChanged();
    void watchedMinuteInPlayer();
    void releasesViewModelChanged();
    void showNextPosterReleaseChanged();
    void nextReleasePosterChanged();
    void seenMarkedAtEndChanged();
    void displaySkipOpeningChanged();

};

#endif // ONLINEPLAYERVIEWMODEL_H
