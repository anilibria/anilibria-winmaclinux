/****************************************************************************
* VLC-Qt - Qt and libvlc connector library
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef VLCQT_QMLPLAYER_H_
#define VLCQT_QMLPLAYER_H_

#include <QtCore/QUrl>

#include "../core/Enums.h"

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;
class VlcTrackModel;

#include "QmlSource.h"

class VlcQmlPlayer : public VlcQmlSource
{
    Q_OBJECT

    Q_PROPERTY(bool autoplay READ autoplay WRITE setAutoplay NOTIFY autoplayChanged)
    Q_PROPERTY(int deinterlacing READ deinterlacing WRITE setDeinterlacing NOTIFY deinterlacingChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(int logLevel READ logLevel WRITE setLogLevel NOTIFY logLevelChanged)
    Q_PROPERTY(qint64 position READ position NOTIFY positionChanged)
    Q_PROPERTY(bool seekable READ seekable NOTIFY seekableChanged)
    Q_PROPERTY(int state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(qreal volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(VlcTrackModel *audioTrackModel READ audioTrackModel CONSTANT)
    Q_PROPERTY(int audioTrack READ audioTrack WRITE setAudioTrack NOTIFY audioTrackChanged)
    Q_PROPERTY(QStringList audioPreferredLanguages READ audioPreferredLanguages WRITE setAudioPreferredLanguages NOTIFY audioPreferredLanguagesChanged)
    Q_PROPERTY(VlcTrackModel *subtitleTrackModel READ subtitleTrackModel CONSTANT)
    Q_PROPERTY(int subtitleTrack READ subtitleTrack WRITE setSubtitleTrack NOTIFY subtitleTrackChanged)
    Q_PROPERTY(QStringList subtitlePreferredLanguages READ subtitlePreferredLanguages WRITE setSubtitlePreferredLanguages NOTIFY subtitlePreferredLanguagesChanged)
    Q_PROPERTY(VlcTrackModel *videoTrackModel READ videoTrackModel CONSTANT)
    Q_PROPERTY(int videoTrack READ videoTrack WRITE setVideoTrack NOTIFY videoTrackChanged)
    Q_PROPERTY(bool isBuffering READ isBuffering NOTIFY isBufferingChanged)
    Q_PROPERTY(bool isEnded READ isEnded NOTIFY isEndedChanged)
    Q_PROPERTY(qreal playbackRate READ playbackRate WRITE setPlaybackRate NOTIFY playbackRateChanged)
    Q_PROPERTY(bool muted READ muted WRITE setMuted NOTIFY mutedChanged)
    Q_PROPERTY(int playbackState READ playbackState NOTIFY playbackStateChanged)

private:
    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;

    bool m_isBuffering { false };
    int m_bufferingProgress { 0 };
    bool m_isEnded { false };
    int m_playbackState { 0 };

    bool _autoplay;
    Vlc::Deinterlacing _deinterlacing;

    VlcTrackModel *_audioTrackModel;
    VlcTrackModel *_subtitleTrackModel;
    VlcTrackModel *_videoTrackModel;

    QStringList _audioPreferredLanguages;
    QStringList _subtitlePreferredLanguages;

public:
    explicit VlcQmlPlayer(QObject *parent = 0);
    ~VlcQmlPlayer();

    bool autoplay() const;
    void setAutoplay(bool autoplay);

    int deinterlacing() const;
    void setDeinterlacing(int deinterlacing);

    qint64 duration() const;
    qint64 position() const;

    int logLevel() const;
    void setLogLevel(int level);

    bool seekable() const;

    int state() const;

    QString source() const noexcept;
    void setSource(QString source) noexcept;

    qreal volume() const;
    void setVolume(qreal volume);

    VlcTrackModel *audioTrackModel() const;

    int audioTrack() const;
    void setAudioTrack(int audioTrack);

    QStringList audioPreferredLanguages() const;
    void setAudioPreferredLanguages(const QStringList &audioPreferredLanguages);

    VlcTrackModel *subtitleTrackModel() const;

    int subtitleTrack() const;
    void setSubtitleTrack(int subtitleTrack);

    QStringList subtitlePreferredLanguages() const;
    void setSubtitlePreferredLanguages(const QStringList &subtitlePreferredLanguages);

    VlcTrackModel *videoTrackModel() const;

    int videoTrack() const;
    void setVideoTrack(int videoTrack);

    qreal playbackRate() const noexcept;
    void setPlaybackRate(qreal playbackRate) noexcept;

    bool isBuffering() const noexcept { return m_isBuffering; }

    bool isEnded() const noexcept { return m_isEnded; }
    void setIsEnded(bool isEnded) noexcept;

    int playbackState() const noexcept { return m_playbackState; }
    void setPlaybackState(int playbackState) noexcept;

    bool muted() const noexcept;
    void setMuted(bool muted) noexcept;

    Q_INVOKABLE void seek(qint64 time);
    Q_INVOKABLE void pause();
    Q_INVOKABLE void play();
    Q_INVOKABLE void stop();

private:
    void openInternal();
    int preferredAudioTrackId();
    int preferredSubtitleTrackId();

private slots:
    void mediaParsed(bool parsed);
    void mediaPlayerVout(int count);
    void buffering(int progress);
    void playerStateChanged();
    void audioMuteChanged(bool muted);

signals:
    void autoplayChanged();
    void deinterlacingChanged();
    void durationChanged();
    void logLevelChanged();
    void positionChanged();
    void seekableChanged();
    void stateChanged();
    void urlChanged();
    void volumeChanged();
    void audioTrackChanged();
    void audioPreferredLanguagesChanged();
    void subtitleTrackChanged();
    void subtitlePreferredLanguagesChanged();
    void videoTrackChanged();
    void isBufferingChanged();
    void isEndedChanged();
    void sourceChanged();
    void playbackRateChanged();
    void sourceLoaded();
    void sourceNotLoaded();
    void mutedChanged();
    void playbackStateChanged();
};

#endif // VLCQT_QMLPLAYER_H_
