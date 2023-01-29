import QtQuick 2.12
import QtAV 1.7

Item {
    id: root
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
    property alias videoPlayerSource: videoPlayer
    property alias videoOutputSource: videoOutput

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
        bufferSize: 400
        fastSeek: true
        timeout: 60000
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

