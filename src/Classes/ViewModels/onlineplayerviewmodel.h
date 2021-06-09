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
#include "../ListModels/onlineplayervideolist.h"

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
    Q_PROPERTY(OnlinePlayerVideoList videos READ videos NOTIFY videosChanged)
    Q_PROPERTY(QString videoSource READ videoSource WRITE setVideoSource NOTIFY videoSourceChanged)
    Q_PROPERTY(QString releasePoster READ releasePoster WRITE setReleasePoster NOTIFY releasePosterChanged)
    Q_PROPERTY(bool isFullHdAllowed READ isFullHdAllowed WRITE setIsFullHdAllowed NOTIFY isFullHdAllowedChanged)
    Q_PROPERTY(int selectedVideo READ selectedVideo WRITE setSelectedVideo NOTIFY selectedVideoChanged)
    Q_PROPERTY(int positionIterator READ positionIterator WRITE setPositionIterator NOTIFY positionIteratorChanged)
    Q_PROPERTY(int lastMovedPosition READ lastMovedPosition WRITE setLastMovedPosition NOTIFY lastMovedPositionChanged)
    Q_PROPERTY(int restorePosition READ restorePosition WRITE setRestorePosition NOTIFY restorePositionChanged)

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

    Q_INVOKABLE void toggleFullScreen();
    Q_INVOKABLE void changeVideoPosition(int duration, int position) noexcept;
    Q_INVOKABLE QString checkExistingVideoQuality(int index);

private:
    QString getZeroBasedDigit(int digit);
    QString getDisplayTimeFromSeconds(int seconds);

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

};

#endif // ONLINEPLAYERVIEWMODEL_H
