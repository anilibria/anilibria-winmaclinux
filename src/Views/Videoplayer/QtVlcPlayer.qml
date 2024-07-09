import QtQuick 2.15
import VLCQt 1.1
import QtMultimedia 5.15

Item {
    id: root
    property alias muted: videoPlayer.muted
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
    property int volume: 0
    property bool isPlaying: false
    property bool isPaused: false
    property bool isStopped: false
    property bool alreadyEnded: false
    property bool isCropped: false

    signal play();
    signal pause();
    signal stop();
    signal seek(real position);
    signal cropModeOutput();
    signal defaultModeOutput();
    signal playerVolumeChanged(int volume);
    signal playerPlaybackStateChanged(string mode);
    signal playerStatusChanged(string status);
    signal playerPositionChanged(bool isBuffered, int position, int duration);
    signal playerBufferProgressChanged();
    signal playerDurationChanged();

    onVolumeChanged: {
        videoPlayer.volume = root.volume / 100;
        playerVolumeChanged(root.volume);
    }

    onCropModeOutput: {
        videoOutput.fillMode = VideoOutput.PreserveAspectCrop;
        root.isCropped = true;
    }

    onDefaultModeOutput: {
        videoOutput.fillMode = VideoOutput.PreserveAspectFit;
        root.isCropped = false;
    }

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
            root.isPlaying = false;
            root.isPaused = false;
            root.isStopped = false;

            let currentMode = "idle";
            if (videoPlayer.playbackState === MediaPlayer.PlayingState) {
                currentMode = "play";
                root.isPlaying = true;
            }
            if (videoPlayer.playbackState === MediaPlayer.PausedState) {
                currentMode = "pause";
                root.isPaused = true;
            }
            if (videoPlayer.playbackState === MediaPlayer.StoppedState) {
                currentMode = "stop";
                root.isStopped = true;
            }

            playerPlaybackStateChanged(currentMode);
        }
        onPositionChanged: {
            playerPositionChanged(!videoPlayer.isBuffering, videoPlayer.position, videoPlayer.duration);
        }
        onDurationChanged: {
            playerDurationChanged();
        }
        onSourceLoaded: {
            playerStatusChanged("loading");
            root.alreadyEnded = false;
        }
        onSourceNotLoaded: {
            playerStatusChanged("invalid");
        }
        onIsEndedChanged: {
            if (root.alreadyEnded) return;

            root.alreadyEnded = true;

            root.isPlaying = false;
            root.isPaused = false;
            root.isStopped = true;

            if (videoPlayer.duration > 0 && videoPlayer.position > 0) playerStatusChanged("endofmedia");
        }
        onIsBufferingChanged: {
            if (isBuffering) {
                playerStatusChanged("buffering");
            } else {
                playerStatusChanged("buffered");
            }
        }
        onEarlyEnded: {
            console.log('early ended!!!!!');
        }
        onSourceChanged: {
            console.log('source changed!!!!!!!');
        }
    }

    VlcVideoOutput {
        id: videoOutput
        source: videoPlayer
        anchors.fill: parent
    }
}

