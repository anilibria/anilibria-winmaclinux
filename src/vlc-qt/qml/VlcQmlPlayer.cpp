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

#include "../core/Audio.h"
#include "../core/Common.h"
#include "../core/Instance.h"
#include "../core/MediaPlayer.h"
#include "../core/Media.h"
#include "../core/TrackModel.h"
#include "../core/Video.h"

#include "VlcQmlPlayer.h"
#include "QmlSource.h"

#include <QDebug>

VlcQmlPlayer::VlcQmlPlayer(QObject *parent)
    : VlcQmlSource(parent),
      _media(0),
      _autoplay(true),
      _deinterlacing(Vlc::Disabled),
      _audioPreferredLanguages(QStringList()),
      _subtitlePreferredLanguages(QStringList())
{
    _instance = new VlcInstance(VlcCommon::args(), this);
    _player = new VlcMediaPlayer(_instance);

    _audioTrackModel = new VlcTrackModel(this);
    _subtitleTrackModel = new VlcTrackModel(this);
    _videoTrackModel = new VlcTrackModel(this);

    connect(_player, &VlcMediaPlayer::lengthChanged, this, &VlcQmlPlayer::durationChanged);
    connect(_player, &VlcMediaPlayer::seekableChanged, this, &VlcQmlPlayer::seekableChanged);
    connect(_player, &VlcMediaPlayer::stateChanged, this, &VlcQmlPlayer::playerStateChanged);
    connect(_player, &VlcMediaPlayer::timeChanged, this, &VlcQmlPlayer::positionChanged);
    connect(_player, &VlcMediaPlayer::bufferingInteger, this, &VlcQmlPlayer::buffering);
    connect(_player, &VlcMediaPlayer::vout, this, &VlcQmlPlayer::mediaPlayerVout);

    setPlayer(_player);
}

VlcQmlPlayer::~VlcQmlPlayer()
{
    _player->stop();
    removePlayer();

    if (_media) delete _media;

    delete _player;
    delete _instance;
}

bool VlcQmlPlayer::autoplay() const
{
    return _autoplay;
}

void VlcQmlPlayer::setAutoplay(bool autoplay)
{
    if (_autoplay == autoplay)
        return;

    _autoplay = autoplay;
    emit autoplayChanged();
}

int VlcQmlPlayer::deinterlacing() const
{
    return _deinterlacing;
}

void VlcQmlPlayer::setDeinterlacing(int deinterlacing)
{
    if (_deinterlacing == deinterlacing)
        return;

    _deinterlacing = Vlc::Deinterlacing(deinterlacing);
    _player->video()->setDeinterlace(_deinterlacing);
    emit deinterlacingChanged();
}

int VlcQmlPlayer::logLevel() const
{
    return _instance->logLevel();
}

void VlcQmlPlayer::setLogLevel(int level)
{
    if (level == logLevel()) return;

    _instance->setLogLevel(Vlc::LogLevel(level));
    emit logLevelChanged();
}

bool VlcQmlPlayer::seekable() const
{
    return _player->seekable();
}

int VlcQmlPlayer::state() const
{
    return _player->state();
}

qint64 VlcQmlPlayer::duration() const
{
    return _player->length();
}

qint64 VlcQmlPlayer::position() const
{
    return _player->time();
}

QString VlcQmlPlayer::source() const noexcept
{
    if (_media) return _media->currentLocation();

    return "";
}

void VlcQmlPlayer::setSource(QString source) noexcept
{
    if (_media && _media->currentLocation() == source) return;

    setIsEnded(false);

    _player->stop();

    if (_media) _media->deleteLater();

    QUrl url(source);

    if (url.isLocalFile()) {
        _media = new VlcMedia(url.toLocalFile(), true, _instance);
    } else {
        _media = new VlcMedia(url.toString(QUrl::FullyEncoded), false, _instance);
    }

    connect(_media, static_cast<void (VlcMedia::*)(bool)>(&VlcMedia::parsedChanged), this, &VlcQmlPlayer::mediaParsed);

    openInternal();

    emit urlChanged();
}

qreal VlcQmlPlayer::volume() const
{
    return m_volume;
}

void VlcQmlPlayer::setVolume(qreal volume)
{
    auto value = static_cast<int>(volume * 100);
    if (value == VlcQmlPlayer::volume()) return;

    _player->audio()->setVolume(value);
    m_volume = volume;
    emit volumeChanged();
}

VlcTrackModel *VlcQmlPlayer::audioTrackModel() const
{
    return _audioTrackModel;
}

int VlcQmlPlayer::audioTrack() const
{
    return _player->audio()->track();
}

void VlcQmlPlayer::setAudioTrack(int audioTrack)
{
    if (audioTrack == VlcQmlPlayer::audioTrack())
        return;

    _player->audio()->setTrack(audioTrack);
    emit audioTrackChanged();
}

QStringList VlcQmlPlayer::audioPreferredLanguages() const
{
    return _audioPreferredLanguages;
}

void VlcQmlPlayer::setAudioPreferredLanguages(const QStringList &audioPreferredLanguages)
{
    if (_audioPreferredLanguages == audioPreferredLanguages)
        return;

    _audioPreferredLanguages = audioPreferredLanguages;
    emit audioPreferredLanguagesChanged();
}

VlcTrackModel *VlcQmlPlayer::subtitleTrackModel() const
{
    return _subtitleTrackModel;
}

int VlcQmlPlayer::subtitleTrack() const
{
    return _player->video()->subtitle();
}

void VlcQmlPlayer::setSubtitleTrack(int subtitleTrack)
{
    if (subtitleTrack == VlcQmlPlayer::subtitleTrack())
        return;

    _player->video()->setSubtitle(subtitleTrack);
    emit subtitleTrackChanged();
}

QStringList VlcQmlPlayer::subtitlePreferredLanguages() const
{
    return _subtitlePreferredLanguages;
}

void VlcQmlPlayer::setSubtitlePreferredLanguages(const QStringList &subtitlePreferredLanguages)
{
    if (_subtitlePreferredLanguages == subtitlePreferredLanguages)
        return;

    _subtitlePreferredLanguages = subtitlePreferredLanguages;
    emit subtitlePreferredLanguagesChanged();
}

VlcTrackModel *VlcQmlPlayer::videoTrackModel() const
{
    return _videoTrackModel;
}

int VlcQmlPlayer::videoTrack() const
{
    return _player->video()->track();
}

void VlcQmlPlayer::setVideoTrack(int videoTrack)
{
    if (videoTrack == VlcQmlPlayer::videoTrack())
        return;

    _player->video()->setTrack(videoTrack);
    emit videoTrackChanged();
}

qreal VlcQmlPlayer::playbackRate() const noexcept
{
    return _player->playbackRate();
}

void VlcQmlPlayer::setPlaybackRate(qreal playbackRate) noexcept
{
    _player->setPlaybackRate(static_cast<float>(playbackRate));
}

void VlcQmlPlayer::setIsEnded(bool isEnded) noexcept
{
    if (m_isEnded == isEnded) return;

    m_isEnded = isEnded;
    emit isEndedChanged();
}

void VlcQmlPlayer::setPlaybackState(int playbackState) noexcept
{
    if (m_playbackState == playbackState) return;

    m_playbackState = playbackState;
    emit playbackStateChanged();
}

bool VlcQmlPlayer::muted() const noexcept
{
    return m_muted;
}

void VlcQmlPlayer::setMuted(bool muted) noexcept
{
    _player->audio()->setMute(muted);
    m_muted = muted;

    emit mutedChanged();
}

void VlcQmlPlayer::mediaParsed(bool parsed)
{
    if (!parsed) {
        emit sourceNotLoaded();
        return;
    }

    _audioTrackModel->clear();
    _audioTrackModel->load(_player->audio()->tracks());

    setAudioTrack(preferredAudioTrackId());

    emit sourceLoaded();
}

void VlcQmlPlayer::mediaPlayerVout(int)
{
    _subtitleTrackModel->clear();
    _subtitleTrackModel->load(_player->video()->subtitles());

    setSubtitleTrack(preferredSubtitleTrackId());

    _videoTrackModel->clear();
    _videoTrackModel->load(_player->video()->tracks());

    setVideoTrack(_player->video()->track());
}

void VlcQmlPlayer::buffering(int progress)
{
    if (m_bufferingProgress == progress) return;

    m_bufferingProgress = progress;

    if (m_isBuffering) {
        if (progress >= 100) {
            m_isBuffering = false;
            emit isBufferingChanged();
        }
    }
    if (!m_isBuffering && progress > 0 && progress < 100) {
        m_isBuffering = true;
        emit isBufferingChanged();
    }
}

void VlcQmlPlayer::playerStateChanged()
{
    auto currentState = state();

    if (currentState == Vlc::Playing) setPlaybackState(1);
    if (currentState == Vlc::Paused) setPlaybackState(2);
    if (currentState != Vlc::Playing && currentState != Vlc::Paused) setPlaybackState(0);

    emit stateChanged();
    setIsEnded(currentState == Vlc::Ended);
}

void VlcQmlPlayer::audioMuteChanged(bool muted)
{
    if (muted) {
    }
    emit mutedChanged();
}

void VlcQmlPlayer::seek(qint64 time)
{
    _player->setTime(time);
}

void VlcQmlPlayer::pause()
{
    _player->pause();
}

void VlcQmlPlayer::play()
{
    _player->play();
}

void VlcQmlPlayer::stop()
{
    _player->stop();
}

void VlcQmlPlayer::openInternal()
{
    if (_autoplay) {
        _player->open(_media);
    } else {
        _player->openOnly(_media);
    }
}

int VlcQmlPlayer::preferredAudioTrackId()
{
    int currentTrackId = _player->audio()->track();
    if (_audioTrackModel->count() && _audioPreferredLanguages.count()) {
        bool found = false;
        for (int j = 0; !found && j < _audioPreferredLanguages.count(); j++) {
            for (int i = 0; !found && i < _audioTrackModel->count(); i++) {
                QString trackTitle = _audioTrackModel->data(i, VlcTrackModel::TitleRole).toString();
                if (trackTitle.contains(_audioPreferredLanguages.at(j))) {
                    currentTrackId = _audioTrackModel->data(i, VlcTrackModel::IdRole).toInt();
                    found = true;
                }
            }
        }
    }

    return currentTrackId;
}

int VlcQmlPlayer::preferredSubtitleTrackId()
{
    int currentTrackId = _player->video()->subtitle();
    if (_subtitleTrackModel->count()) {
        bool found = false;
        for (int j = 0; !found && j < _subtitlePreferredLanguages.count(); j++) {
            for (int i = 0; !found && i < _subtitleTrackModel->count(); i++) {
                QString trackTitle = _subtitleTrackModel->data(i, VlcTrackModel::TitleRole).toString();
                if (trackTitle.contains(_subtitlePreferredLanguages.at(j))) {
                    currentTrackId = _subtitleTrackModel->data(i, VlcTrackModel::IdRole).toInt();
                    found = true;
                }
            }
        }
    }

    return currentTrackId;
}
