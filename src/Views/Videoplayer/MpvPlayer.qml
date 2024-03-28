import QtQuick 2.12
import MpvIntegration 1.0

Item {
    id: root
    property alias muted: videoPlayer.muted
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
    property int volume: 0
    property bool isPlaying: false
    property bool isPaused: false
    property bool isStopped: false
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
        videoPlayer.volume = root.volume;
        playerVolumeChanged(root.volume);
    }

    onCropModeOutput: {
        //videoOutput.fillMode = VideoOutput.PreserveAspectCrop;
        root.isCropped = true;
    }

    onDefaultModeOutput: {
        //videoOutput.fillMode = VideoOutput.PreserveAspectFit;
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

    MpvObject {
        id: videoPlayer
        onPositionChanged: {
            playerPositionChanged(true, videoPlayer.position, videoPlayer.duration);
        }
        onDurationChanged: {
            playerDurationChanged();
        }
        onEndFileReached: {
            playerStatusChanged("endofmedia");
        }
        onFileLoaded: {
            playerStatusChanged("loading");
        }
        onStartBuffering: {
            playerStatusChanged("buffering");
        }
        onEndBuffered: {
            playerStatusChanged("buffered");
        }
        onPlaybackChanged: function (newPlayback) {
            root.isPlaying = false;
            root.isPaused = false;
            root.isStopped = false;

            switch (newPlayback) {
                case 1:
                    root.isPaused = true;
                    playerPlaybackStateChanged("pause");
                    break;
                case 2:
                    root.isPlaying = true;
                    playerPlaybackStateChanged("play");
                    break;
                case 3:
                    root.isStopped = true;
                    playerPlaybackStateChanged("stop");
                    break;
            }
        }
    }

}
