import QtQuick 2.12
import VLCQt 1.1
import QtMultimedia 5.12

Item {
    id: root
    property alias muted: videoPlayer.muted
    property alias volume: videoPlayer.volume
    property alias position: videoPlayer.position
    property alias duration: videoPlayer.duration
    property alias playbackState: videoPlayer.playbackState
    property int status: 0
    property int bufferProgress: 0
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

    VlcPlayer {
        id: videoPlayer
        autoplay: true
        logLevel: 4
        onPlaybackStateChanged: {
            playerPlaybackStateChanged();
        }
        onVolumeChanged: {
            playerVolumeChanged();
        }
        onPositionChanged: {
            playerPositionChanged();
        }
        onDurationChanged: {
            playerDurationChanged();
        }
        onSourceLoaded: {
            root.status = MediaPlayer.Loading;
        }
        onSourceNotLoaded: {
            root.status = MediaPlayer.InvalidMedia;
        }
        onIsEndedChanged: {
            root.status = MediaPlayer.EndOfMedia;
        }
        onIsBufferingChanged: {
            if (isBuffering) {
                root.status = MediaPlayer.Buffering;
            } else {
                root.status = MediaPlayer.Buffered;
            }
        }
        onEarlyEnded: {
            onlinePlayerViewModel.restorePosition = time;
            reloadCurrentSource();
            console.log('early ended!!!!!');
        }
    }

    VlcVideoOutput {
        id: videoOutput
        source: videoPlayer
        anchors.fill: parent
    }
}

