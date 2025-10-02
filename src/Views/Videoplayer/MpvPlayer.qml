import QtQuick 2.15
import MpvIntegration 1.0

Item {
    id: root
    property alias muted: videoPlayer.muted
    property alias position: videoPlayer.position
    property alias duration: videoPlayer.duration
    property int playbackState: 0
    property int status: 0//videoPlayer.status
    property int bufferProgress: 0
    property alias source: videoPlayer.source
    property alias playbackRate: videoPlayer.playbackRate
    property int fillMode: 0
    property alias videoPlayerSource: videoPlayer
    property alias videoOutputSource: fakeOutput
    property int volume: 0
    property bool isPlaying: false
    property bool isPaused: false
    property bool isStopped: false
    property bool isCropped: false
    property alias selectedAudio: videoPlayer.audioTrack
    property alias selectedSubtitle: videoPlayer.subtitleTrack
    property var subtitles: ['Без субтитров']
    property var audios: ['Дорожка 1']

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
        videoPlayer.setCropMode();
        root.isCropped = true;
    }

    onDefaultModeOutput: {
        videoPlayer.revertCropMode();
        root.isCropped = false;
    }

    onWidthChanged: {
        if (root.isCropped) videoPlayer.setCropMode();
    }

    onHeightChanged: {
        if (root.isCropped) videoPlayer.setCropMode();
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

    Item {
        id: fakeOutput
    }

    MpvObject {
        id: videoPlayer
        anchors.fill: parent
        onPositionChanged: {
            playerPositionChanged(true, videoPlayer.position, videoPlayer.duration);
        }
        onDurationChanged: {
            playerDurationChanged();
        }
        onEndFileReached: {
            playerStatusChanged("endofmedia");
            root.isPlaying = false;
            root.isPaused = false;
            root.isStopped = false;
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

        onCountAudioTrackChanged: {
            if (videoPlayer.countAudioTrack === 1) {
                root.audios = ['Дорожка 1'];
            } else {
                let arr = [];
                for (let i = 0; i < videoPlayer.countAudioTrack; i++) {
                    arr.push("Дорожка " + (i + 1));
                }

                root.audios = arr;
            }

        }
        onCountSubtitleTrackChanged: {
            let arr = ['Без субтитров'];
            for (let i = 1; i < videoPlayer.countSubtitleTrack; i++) {
                arr.push("Субтитры " + i);
            }

            root.subtitles = arr
        }
    }

}
