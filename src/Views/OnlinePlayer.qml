import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtMultimedia 5.12
import "../Controls"

Page {
    id: _page
    property bool isFullScreen: false
    property var selectedRelease: null
    property string videoSource: ""
    property var releaseVideos: []
    property var selectedVideo: null
    property bool isFullHdAllowed: false
    property real lastMovedPosition: 0
    property real restorePosition: 0
    property string videoQuality: "sd"
    property string displayVideoPosition: "00:00:00"
    property string displayEndVideoPosition: "00:00:00"
    property bool isBuffering: false
    property var setReleaseParameters: ({})
    property double videoSpeed: 1
    property int positionIterator: 0
    property var seenVideo: ({})
    property var seenMarks: ({})
    property int prefferedQuality: 0 // 0 - 480, 1 - 720p, 2 - 1080p

    signal navigateFrom()
    signal setReleaseVideo()
    signal changeFullScreenMode(bool fullScreen)
    signal navigateTo()

    Keys.onSpacePressed: {
        if (player.playbackState === MediaPlayer.PlayingState) {
            player.pause();
            return;
        }
        if (player.playbackState === MediaPlayer.PausedState || player.playbackState === MediaPlayer.StoppedState) {
            player.play();
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_PageUp) {
            _page.nextVideo();
            event.accepted = true;
        }
        if (event.key === Qt.Key_PageDown) {
            _page.previousVideo();
            event.accepted = true;
        }
    }

    onNavigateFrom: {
        player.pause();
    }

    onNavigateTo: {
        if (!_page.setReleaseParameters.releaseId) {
            const lastSeenObject = localStorage.getLastVideoSeen();
            if (lastSeenObject === ``) return;

            const seenObject = JSON.parse(lastSeenObject);
            const release = JSON.parse(localStorage.getRelease(seenObject.id));
            console.log(release);
            _page.setReleaseParameters = {
                releaseId: seenObject.id,
                videos: release.videos,
                customPlaylistPosition: -1
            };

            _page.setReleaseVideo();
        }
        const userSettings = JSON.parse(localStorage.getUserSettings());
        console.log(userSettings.autoNextVideo);
        player.volume = userSettings.volume;
        autoNextVideo.checked = userSettings.autoNextVideo;
        autoTopMost.checked = userSettings.autoTopMost;
        _page.prefferedQuality = userSettings.quality;
        switch (userSettings.quality) {
            case 0:
                _page.videoQuality = "sd";
                break;
            case 1:
                _page.videoQuality = "hd";
                break;
            case 2:
                _page.videoQuality = "fullhd";
                break;
        }
    }

    onSetReleaseVideo: {
        const jsonVideos = JSON.parse(_page.setReleaseParameters.videos);
        const seenJson = localStorage.getVideoSeen(_page.setReleaseParameters.releaseId);
        _page.seenVideo = {};
        if (seenJson) _page.seenVideo = JSON.parse(seenJson);

        const releaseVideos = [];

        for (let i = 0; i < jsonVideos.length; i++) {
            const video = jsonVideos[i];
            releaseVideos.push({ title: video.title, sd: video.sd, id: video.id, hd: video.hd, fullhd: video.fullhd });
        }
        releaseVideos.sort(
            (left, right) => {
                if (left.id === right.id) return 0;
                return left.id > right.id ? 1 : -1;
            }
        );
        let iterator = -1;
        releaseVideos.forEach(
            a => {
                a.order = ++iterator;
            }
        );

        _page.releaseVideos = releaseVideos;
        let firstVideo = releaseVideos[0];
        if (_page.seenVideo.id) {
            firstVideo = releaseVideos[_page.seenVideo.videoId];
        }

        refreshSeenMarks();

        _page.selectedVideo = firstVideo.order;
        _page.isFullHdAllowed = "fullhd" in firstVideo;
        if (!firstVideo[_page.videoQuality]) _page.videoQuality = "sd";

        _page.videoSource = firstVideo[_page.videoQuality];
        player.play();

        localStorage.setToReleaseHistory(_page.setReleaseParameters.releaseId, 1);
    }

    function getZeroBasedDigit(digit) {
        if (digit < 10) return `0${digit}`;
        return `${digit}`;
    }

    function getDisplayTimeFromSeconds(seconds) {
        const days = Math.floor(seconds / (3600 * 24));
        seconds -= days * 3600 * 24;
        const hours = Math.floor(seconds / 3600);
        seconds -= hours * 3600;
        const minutes = Math.floor(seconds / 60);
        seconds  -= minutes * 60;

        return `${getZeroBasedDigit(hours)}:${getZeroBasedDigit(minutes)}:${getZeroBasedDigit(Math.round(seconds))}`;
    }

    function setControlVisible(visible) {
        if (visible) {
            seriesPopup.opacity = 1;
            controlPanel.opacity = 1;
            mainPlayerMouseArea.cursorShape = "ArrowCursor";
        } else {
            seriesPopup.opacity = 0;
            controlPanel.opacity = 0;
            mainPlayerMouseArea.cursorShape = "BlankCursor";
        }
    }

    anchors.fill: parent

    background: Rectangle {
        color: "black"
    }

    Timer {
        id: playerTimer
        interval: 2000
        running: false
        repeat: true
        onTriggered: _page.setControlVisible(false)
    }

    MediaPlayer {
        id: player
        source: _page.videoSource
        autoPlay: true
        playbackRate: _page.videoSpeed
        onPlaying: {
            console.log("playing changed");
        }
        onStopped: {
            console.log("stopped changed");
        }
        onPlaybackStateChanged: {
            playButton.visible = playbackState === MediaPlayer.PausedState || playbackState === MediaPlayer.StoppedState;
            pauseButton.visible = playbackState === MediaPlayer.PlayingState;
            if (playbackState === MediaPlayer.PlayingState) {
                playerTimer.start();
            } else {
                playerTimer.stop();
                _page.setControlVisible(true);
            }
        }
        onVolumeChanged: {
            volumeSlider.value = volume * 100;
        }
        onStatusChanged: {
            if (status === MediaPlayer.Loading) _page.isBuffering = true;

            if (status === MediaPlayer.EndOfMedia) _page.nextVideo();

            if (status === MediaPlayer.InvalidMedia) {
                console.log("InvalidMedia")
            }

            if (status === MediaPlayer.Buffering) _page.isBuffering = true;

            if (status === MediaPlayer.Buffered) {
                _page.isBuffering = false;
                if (_page.restorePosition > 0){
                    player.seek(_page.restorePosition);
                    _page.restorePosition = 0;
                } else if (_page.seenVideo.id && _page.seenVideo.videoPosition) {
                    player.seek(_page.seenVideo.videoPosition);
                    _page.seenVideo.videoPosition = 0;
                }
            }
        }

        onPositionChanged: {
            if (!playerLocation.pressed) playerLocation.value = position;

            _page.displayVideoPosition = `${_page.getDisplayTimeFromSeconds(position / 1000)} из ${_page.getDisplayTimeFromSeconds(duration / 1000)}`;
            _page.displayEndVideoPosition = _page.getDisplayTimeFromSeconds((duration - position) / 1000);

            if (_page.positionIterator < 20) _page.positionIterator++;

            if (_page.positionIterator >= 20) {
                _page.positionIterator = 0;
                localStorage.setVideoSeens(_page.setReleaseParameters.releaseId, _page.selectedVideo, position);
            }

            if (!(_page.selectedVideo in _page.seenMarks)) {
                if (duration > 0 && position > 0) {
                    const positionPercent = position / duration * 100;
                    console.log("percent", positionPercent);
                    if (positionPercent >= 90) {
                        const obj = _page.seenMarks;
                        obj[_page.selectedVideo] = true;
                        _page.seenMarks = obj;
                        localStorage.setSeenMark(_page.setReleaseParameters.releaseId, _page.selectedVideo, true);
                    }
                }
            }
        }
    }

    MouseArea {
        id: mainPlayerMouseArea
        anchors.fill: parent
        hoverEnabled: true
        onDoubleClicked: {
            toggleFullScreen();
        }
        onPositionChanged: {
            if (player.playbackState === MediaPlayer.PlayingState) {
                _page.setControlVisible(true);
                playerTimer.restart();
            }
        }
    }
    VideoOutput {
        id: videoOutput
        source: player
        anchors.fill: parent
    }

    Rectangle {
        id: seriesPopup
        anchors.top: parent.top
        width: 140
        height: _page.height - controlPanel.height - 20
        color: "transparent"

        Flickable {
            width: seriesPopup.width
            height: seriesPopup.height
            contentWidth: seriesPopup.width
            contentHeight: itemsContent.height
            clip: true

            ScrollBar.vertical: ScrollBar {
                active: true
            }

            Column {
                id: itemsContent
                Repeater {
                    model: _page.releaseVideos
                    delegate: Row {
                        Rectangle {
                            height: 40
                            width: seriesPopup.width
                            color: _page.selectedVideo === modelData.order ? "#64c25656" : "#C8ffffff"
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    _page.selectedVideo = modelData.order;
                                    _page.isFullHdAllowed = "fullhd" in modelData;
                                    _page.videoSource = modelData[_page.videoQuality];
                                    player.play();
                                }
                            }
                            Text {
                                color: _page.selectedVideo === modelData.order ? "white" : "black"
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                text: modelData.title
                            }

                            IconButton {
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                height: 36
                                width: 36
                                iconColor: "black"
                                hoverColor: "white"
                                iconPath: modelData.order in _page.seenMarks ? "../Assets/Icons/seenmarkselected.svg" : "../Assets/Icons/seenmark.svg"
                                iconWidth: 22
                                iconHeight: 22
                                onButtonPressed: {
                                    let newState = false;
                                    if (_page.seenMarks[modelData.order]) {
                                        const obj = _page.seenMarks;
                                        delete obj[modelData.order];
                                        _page.seenMarks = obj;
                                        newState = false;
                                    } else {
                                        const obj = _page.seenMarks;
                                        obj[modelData.order] = true;
                                        _page.seenMarks = obj;
                                        newState = true;

                                    }
                                    localStorage.setSeenMark(_page.setReleaseParameters.releaseId, modelData.order, newState);
                                }
                            }
                        }
                    }
                }
            }
        }

        Behavior on opacity {
            NumberAnimation { duration: 200 }
        }
    }

    Rectangle {
        id: controlPanel
        color: "#82ffffff"
        anchors.bottom: parent.bottom
        width: _page.width
        height: 100

        Column {
            width: controlPanel.width

            Slider {
                id: playerLocation
                height: 20
                width: controlPanel.width
                from: 1
                value: 1
                to: player.duration
                onPressedChanged: {
                    if (!pressed && _page.lastMovedPosition > 0) {
                        player.seek(_page.lastMovedPosition);
                        _page.lastMovedPosition = 0;
                    }
                }

                onMoved: {
                    if (pressed) _page.lastMovedPosition = value;
                }
            }

            Item {
                height: 20
                width: controlPanel.width

                Text {
                    text: _page.displayVideoPosition
                }

                Row {
                    height: 20
                    anchors.horizontalCenter: parent.horizontalCenter
                    ToggleButton {
                        height: 20
                        width: 60
                        text: "1080p"
                        visible: _page.isFullHdAllowed
                        isChecked: _page.videoQuality === `fullhd`
                        onButtonClicked: {
                            _page.videoQuality = `fullhd`;
                            _page.restorePosition = player.position;

                            const video = _page.releaseVideos.find(a => a.order === _page.selectedVideo);

                            player.stop();
                            _page.videoSource = video[_page.videoQuality];

                            localStorage.setVideoQuality(2);
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 60
                        text: "720p"
                        isChecked: _page.videoQuality === `hd`
                        onButtonClicked: {
                            _page.videoQuality = `hd`;
                            _page.restorePosition = player.position;

                            const video = _page.releaseVideos.find(a => a.order === _page.selectedVideo);

                            player.stop();
                            _page.videoSource = video[_page.videoQuality];

                            localStorage.setVideoQuality(1);
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 60
                        text: "480p"
                        isChecked: _page.videoQuality === `sd`
                        onButtonClicked: {
                            _page.videoQuality = `sd`;
                            _page.restorePosition = player.position;

                            const video = _page.releaseVideos.find(a => a.order === _page.selectedVideo);

                            player.stop();
                            _page.videoSource = video[_page.videoQuality];

                            localStorage.setVideoQuality(0);
                        }
                    }
                    Rectangle {
                        width: 20
                        height: 20
                        color: "transparent"
                        Text {
                            anchors.centerIn: parent
                            text: "|"
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        text: "x1"
                        isChecked: _page.videoSpeed === 1
                        onButtonClicked: {
                            _page.videoSpeed = 1;
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        text: "x2"
                        isChecked: _page.videoSpeed === 1.5
                        onButtonClicked: {
                            _page.videoSpeed = 1.5;
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        text: "x3"
                        isChecked: _page.videoSpeed === 2
                        onButtonClicked: {
                            _page.videoSpeed = 2;
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        text: "x4"
                        isChecked: _page.videoSpeed === 3
                        onButtonClicked: {
                            _page.videoSpeed = 3;
                        }
                    }
                }

                Text {
                    height: 20
                    anchors.right: parent.right
                    anchors.rightMargin: 4
                    text: _page.displayEndVideoPosition
                }
            }

            Item {
                height: 60
                width: controlPanel.width

                Row {
                    spacing: 5
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    IconButton {
                        width: 40
                        height: 40
                        iconColor: "black"
                        iconPath: "../Assets/Icons/menu.svg"
                        iconWidth: 29
                        iconHeight: 29
                        onButtonPressed: {
                            drawer.open();
                        }
                    }
                    IconButton {
                        width: 40
                        height: 40
                        iconColor: "black"
                        iconPath: "../Assets/Icons/speaker.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            player.muted = !player.muted;
                        }
                    }
                    Slider {
                        width: 60
                        height: 40
                        id: volumeSlider
                        from: 0
                        value: 10
                        to: 100
                        onPressedChanged: {
                            if (!pressed) {
                                localStorage.setVolume(player.volume);
                            }
                        }
                        onMoved: {
                            player.volume = value / 100;
                        }
                    }
                }

                Row {
                    anchors.centerIn: parent
                    spacing: 5
                    IconButton {
                        width: 40
                        height: 40
                        iconColor: "black"
                        iconPath: "../Assets/Icons/step-backward.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            _page.previousVideo();
                        }
                    }
                    IconButton {
                        id: playButton
                        width: 40
                        height: 40
                        iconColor: "black"
                        iconPath: "../Assets/Icons/play-button.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            player.play();
                        }
                    }
                    IconButton {
                        id: pauseButton
                        width: 40
                        height: 40
                        iconColor: "black"
                        iconPath: "../Assets/Icons/pause.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            player.pause();
                        }
                    }
                    IconButton {
                        width: 40
                        height: 40
                        iconColor: "black"
                        iconPath: "../Assets/Icons/step-forward.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            _page.nextVideo();
                        }
                    }
                }

                Row {
                    spacing: 5
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter

                    IconButton {
                        id: optionsButton
                        width: 40
                        height: 40
                        iconColor: "black"
                        iconPath: "../Assets/Icons/options.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            optionsPopup.open();
                        }

                        Popup {
                            id: optionsPopup
                            x: optionsButton.width - 300
                            y: optionsButton.height - 200
                            width: 300
                            height: 180

                            modal: true
                            focus: true
                            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                            Column {
                                width: parent.width
                                spacing: 10
                                Text {
                                    width: optionsPopup.width - 20
                                    font.pixelSize: 12
                                    text: "Автопереход между сериями"
                                }

                                Switch {
                                    id: autoNextVideo
                                    onCheckedChanged: {
                                        localStorage.setAutoNextVideo(checked);
                                    }
                                }

                                Text {
                                    width: optionsPopup.width - 20
                                    font.pixelSize: 12
                                    text: "Автопереход в режим поверх всех окон"
                                }

                                Switch {
                                    id: autoTopMost
                                    onCheckedChanged: {
                                        localStorage.setAutoTopMost(checked);
                                    }
                                }
                            }
                        }

                    }
                    IconButton {
                        width: 40
                        height: 40
                        iconColor: "black"
                        iconPath: "../Assets/Icons/resize.svg"
                        iconWidth: 29
                        iconHeight: 29
                        onButtonPressed: {
                            switch (videoOutput.fillMode) {
                                case VideoOutput.PreserveAspectFit:
                                    videoOutput.fillMode = VideoOutput.PreserveAspectCrop;
                                    break;
                                case VideoOutput.PreserveAspectCrop:
                                    videoOutput.fillMode = VideoOutput.PreserveAspectFit;
                                    break;
                            }
                        }
                    }
                    IconButton {
                        width: 40
                        height: 40
                        iconColor: "black"
                        iconPath: "../Assets/Icons/fullscreen.svg"
                        iconWidth: 29
                        iconHeight: 29
                        onButtonPressed: {
                            toggleFullScreen();
                        }
                    }
                }
            }
        }

        Behavior on opacity {
            NumberAnimation { duration: 200 }
        }
    }

    Rectangle {
        width: 80
        height: 80
        color: "white"
        radius: 20
        opacity: 0.8
        visible: _page.isBuffering
        anchors.centerIn: parent
        AnimatedImage {
            id: spinner
            anchors.centerIn: parent
            source: "../Assets/Icons/spinner.gif"
        }
    }

    function checkExistingVideoQuality() {
        const video = _page.releaseVideos[_page.selectedVideo];
        if (video[_page.videoQuality]) {
            return _page.releaseVideos[_page.selectedVideo][_page.videoQuality];
        } else {
            if (`sd` in _page.releaseVideos[_page.selectedVideo]) {
                _page.videoQuality = `sd`;
                return _page.releaseVideos[_page.selectedVideo]['sd'];
            }
            if (`hd` in _page.releaseVideos[_page.selectedVideo]) {
                _page.videoQuality = `hd`;
                return _page.releaseVideos[_page.selectedVideo]['hd'];
            }
            if (`fullhd` in _page.releaseVideos[_page.selectedVideo]) {
                _page.videoQuality = `fullhd`;
                return _page.releaseVideos[_page.selectedVideo]['fullhd'];
            }

            return null;
        }
    }

    function previousVideo() {
        if (_page.selectedVideo === 0) return;
        _page.restorePosition = 0;

        _page.selectedVideo--;
        const video = _page.releaseVideos[_page.selectedVideo];
        _page.isFullHdAllowed = "fullhd" in video;

        _page.videoSource = checkExistingVideoQuality();
    }

    function nextVideo() {
        if (_page.selectedVideo === _page.releaseVideos.length) return;
        _page.restorePosition = 0;

        _page.selectedVideo++;
        const video = _page.releaseVideos[_page.selectedVideo];
        _page.isFullHdAllowed = "fullhd" in video;

        _page.videoSource = checkExistingVideoQuality();
    }

    function toggleFullScreen() {
        isFullScreen = !isFullScreen;
        changeFullScreenMode(isFullScreen);
    }

    function refreshSeenMarks() {
        const releaseSeenMarks = {};
        const seenMarks = localStorage.getReleseSeenMarks(_page.setReleaseParameters.releaseId, _page.releaseVideos.length)
        for (const seenMark of seenMarks) {
            const key = seenMark.toString();
            releaseSeenMarks[key] = true;
        }
        _page.seenMarks = releaseSeenMarks;
    }

    Component.onCompleted: {
        volumeSlider.value = player.volume * 100;
    }

}

