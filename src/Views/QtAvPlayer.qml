/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2020 Roman Vladimirov

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

import QtQuick 2.12
import QtAV 1.7

Item {
    property alias muted: videoPlayer.muted
    property alias volume: videoPlayer.volume
    property alias position: videoPlayer.position
    property alias duration: videoPlayer.duration
    property alias playbackState: videoPlayer.playbackState
    property alias status: videoPlayer.status
    property alias bufferProgress: videoPlayer.bufferProgress
    property alias source: videoPlayer.source
    property alias playbackRate: videoPlayer.playbackRate
    property alias fillMode: videoOutput.fillMode

    signal play();
    signal pause();
    signal stop();
    signal seek(real position);
    signal playerVolumeChanged();
    signal playerPlaybackStateChanged();
    signal playerStatusChanged();
    signal playerPositionChanged();
    signal playerBufferProgressChanged();
    signal playerDurationChanged();

    onPlay: {
        videoPlayer.play();
    }

    onPause: {
        videoPlayer.pause();
    }

    onStop: {
        videoPlayer.stop();
    }

    onSeek: {
        videoPlayer.seek(position);
    }

    VideoOutput2 {
        id: videoOutput
        anchors.fill: parent
        source: videoPlayer
    }

    MediaPlayer {
        id: videoPlayer
        autoPlay: true
        onBufferProgressChanged: {
            playerBufferProgressChanged();
        }
        onPlaybackStateChanged: {
            playerPlaybackStateChanged();
        }
        onVolumeChanged: {
            playerVolumeChanged();
        }
        onStatusChanged: {
            playerStatusChanged();
        }
        onPositionChanged: {
            playerPositionChanged();
        }
        onDurationChanged: {
            playerDurationChanged();
        }
    }
}

