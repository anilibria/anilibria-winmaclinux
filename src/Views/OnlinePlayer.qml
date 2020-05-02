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
    property var jumpMinutes: [0, 1, 2]
    property var jumpSeconds: [0, 5, 10, 15, 20, 25, 30]
    property real lastMouseYPosition: 0

    signal navigateFrom()
    signal setReleaseVideo()
    signal changeFullScreenMode(bool fullScreen)
    signal navigateTo()
    signal returnToReleasesPage()
    signal windowNotActived()

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
        if (event.key === Qt.Key_Escape) {
            //changepage to releases
        }
        if (event.key === Qt.Key_F11 || event.key === Qt.Key_F || event.key === 1040) {
            toggleFullScreen();
        }
        if (event.key === Qt.Key_Up) {
            if (player.volume < 1) player.volume += .1;
            if (player.volume > 1) player.volume = 1;

            volumeSlider.value = player.volume * 100;
        }
        if (event.key === Qt.Key_Down) {
            if (player.volume > 0) player.volume -= .1;
            if (player.volume < 0) player.volume = 0;

            volumeSlider.value = player.volume * 100;
        }
        if (event.key === Qt.Key_M || event.key === 1068) player.muted = !player.muted;
        if ((event.key === Qt.Key_P || event.key === 1047) && !autoTopMost.checked) windowSettings.toggleStayOnTopMode();
        if (event.key === Qt.Key_Left) jumpInPlayer(true);
        if (event.key === Qt.Key_Right) jumpInPlayer(false);
        if (event.key === Qt.Key_Escape) returnToReleasesPage();
    }

    onWindowNotActived: {
        if (!playerTimer.running) playerTimer.restart();
    }

    onNavigateFrom: {
        windowSettings.unsetStayOnTop();
        player.pause();
        disableFullScreen();
    }

    onNavigateTo: {
        const userSettings = JSON.parse(localStorage.getUserSettings());
        player.volume = userSettings.volume;
        autoNextVideo.checked = userSettings.autoNextVideo;
        autoTopMost.checked = userSettings.autoTopMost;
        jumpMinuteComboBox.currentIndex = jumpMinutes.indexOf(userSettings.jumpMinute);
        jumpSecondComboBox.currentIndex = jumpSeconds.indexOf(userSettings.jumpSecond);

        if (autoTopMost.checked && player.playbackState === MediaPlayer.PlayingState) windowSettings.setStayOnTop();
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

        if (!_page.setReleaseParameters.releaseId) {
            const lastSeenObject = localStorage.getLastVideoSeen();
            if (lastSeenObject === ``) return;

            const seenObject = JSON.parse(lastSeenObject);
            const release = JSON.parse(localStorage.getRelease(seenObject.id));
            _page.setReleaseParameters = {
                releaseId: seenObject.id,
                videos: release.videos,
                customPlaylistPosition: -1
            };

            _page.setReleaseVideo();
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
            releaseVideos.push({ title: video.title, sd: video.sd, id: video.id, hd: video.hd, fullhd: video.fullhd, sdfile: video.srcSd, hdfile: video.srcHd });
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
        if (_page.setReleaseParameters.customPlaylistPosition > -1) {
            _page.seenVideo = {};
            firstVideo = releaseVideos[_page.setReleaseParameters.customPlaylistPosition];
        }

        refreshSeenMarks();

        _page.selectedVideo = firstVideo.order;
        _page.isFullHdAllowed = "fullhd" in firstVideo;
        if (!firstVideo[_page.videoQuality]) _page.videoQuality = "sd";

        _page.videoSource = firstVideo[_page.videoQuality];
        player.play();

        localStorage.setToReleaseHistory(_page.setReleaseParameters.releaseId, 1);

        setSerieScrollPosition();
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

    MouseArea {
        id: mainPlayerMouseArea
        anchors.fill: parent
        hoverEnabled: true
        onDoubleClicked: {
            toggleFullScreen();
        }
        onPositionChanged: {
            if (!(player.playbackState === MediaPlayer.PlayingState)) {
                if (controlPanel.opacity === 0) _page.setControlVisible(true);
                return;
            }

            _page.setControlVisible(true);
            const x = mouse.x;
            const y = mouse.y;
            _page.lastMouseYPosition = y;
            if (y > _page.height - controlPanel.height) {
                playerTimer.stop();
            } else {
                playerTimer.restart();
            }
        }
        onExited: {
            if (_page.height - _page.lastMouseYPosition < 10) if (!playerTimer.running) playerTimer.restart();
        }
    }

    QtPlayer {
        id: player
        anchors.fill: parent
        source: _page.videoSource
        playbackRate: _page.videoSpeed
        onBufferProgressChanged: {
            //_page.isBuffering = bufferProgress < 1;
        }
        onPlaybackStateChanged: {
            if (playbackState === MediaPlayer.PlayingState && autoTopMost.checked) {
                windowSettings.setStayOnTop();
            } else {
                if (autoTopMost.checked) windowSettings.unsetStayOnTop();
            }

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

            if (status === MediaPlayer.EndOfMedia && autoNextVideo.checked) _page.nextVideo();

            if (status === MediaPlayer.InvalidMedia) {
                console.log("InvalidMedia")
            }

            if (status === MediaPlayer.Buffering) _page.isBuffering = true;

            if (status === MediaPlayer.Buffered) {                
                _page.isBuffering = false;                
                if (_page.restorePosition > 0){
                    player.seek(_page.restorePosition);
                    if (player.position >= _page.restorePosition) _page.restorePosition = 0;
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

    Rectangle {
        id: seriesPopup
        anchors.top: parent.top
        width: 200
        height: _page.height - controlPanel.height - 20
        color: "transparent"

        Flickable {
            id: serieScrollContainer
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
                                hoverEnabled: true
                                onEntered: {
                                    if (playerTimer.running) playerTimer.stop();
                                }
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

                            Row {
                                width: 72
                                spacing: 0
                                anchors.right: parent.right
                                anchors.rightMargin: 18

                                IconButton {
                                    height: 36
                                    width: 36
                                    iconColor: "black"
                                    hoverColor: "white"
                                    iconPath: modelData.order in _page.seenMarks ? "../Assets/Icons/seenmarkselected.svg" : "../Assets/Icons/seenmark.svg"
                                    iconWidth: 22
                                    iconHeight: 22
                                    onButtonHoverEnter: {
                                        if (playerTimer.running) playerTimer.stop();
                                    }
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

                                IconButton {
                                    height: 36
                                    width: 36
                                    iconColor: "black"
                                    hoverColor: "white"
                                    iconPath: "../Assets/Icons/download.svg"
                                    iconWidth: 22
                                    iconHeight: 22
                                    onButtonHoverEnter: {
                                        if (playerTimer.running) playerTimer.stop();
                                    }
                                    onButtonPressed: {
                                        switch (_page.videoQuality) {
                                            case "fullhd":
                                            case "hd":
                                                Qt.openUrlExternally(modelData.hdfile);
                                                break;
                                            case "sd":
                                                Qt.openUrlExternally(modelData.sdfile);
                                                break;
                                        }
                                    }
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
            height: 100

            Slider {
                id: playerLocation
                visible: player.duration > 0
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
                    controlPanel.forceActiveFocus();
                }

                onMoved: {
                    if (pressed) _page.lastMovedPosition = value;
                }
            }

            Rectangle {
                width: 2
                height: 20
                color: "transparent"
                visible: player.duration === 0
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
                        visible: player.duration > 0 && _page.isFullHdAllowed
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
                        visible: player.duration > 0
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
                        visible: player.duration > 0
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
                        visible: player.duration > 0
                        color: "transparent"
                        Text {
                            anchors.centerIn: parent
                            text: "|"
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x1"
                        isChecked: _page.videoSpeed === 1
                        onButtonClicked: {
                            _page.videoSpeed = 1;
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x2"
                        isChecked: _page.videoSpeed === 1.5
                        onButtonClicked: {
                            _page.videoSpeed = 1.5;
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x3"
                        isChecked: _page.videoSpeed === 2
                        onButtonClicked: {
                            _page.videoSpeed = 2;
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
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
                            controlPanel.forceActiveFocus();
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
                        visible: player.duration > 0
                        iconColor: "black"
                        iconPath: "../Assets/Icons/previous10.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            player.seek(player.position - 10 * 1000);
                        }
                    }
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
                        visible: false
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
                        visible: false
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
                    IconButton {
                        width: 40
                        height: 40
                        visible: player.duration > 0
                        iconColor: "black"
                        iconPath: "../Assets/Icons/next30.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            player.seek(player.position + 30 * 1000);
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
                            y: optionsButton.height - 250
                            width: 300
                            height: 250

                            modal: true
                            focus: true
                            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                            Column {
                                width: parent.width
                                spacing: 10
                                Text {
                                    width: optionsPopup.width - 20
                                    font.pointSize: 10
                                    text: "Время прыжка"
                                }

                                Grid {
                                    columns: 2
                                    spacing: 2

                                    ComboBox {
                                        id: jumpMinuteComboBox
                                        Layout.column: 0
                                        model: ListModel {
                                            ListElement {
                                                text: "0"
                                            }
                                            ListElement {
                                                text: "1"
                                            }
                                            ListElement {
                                                text: "2"
                                            }
                                        }
                                        onActivated: {
                                            localStorage.setJumpMinute(_page.jumpMinutes[index]);
                                        }
                                    }
                                    ComboBox {
                                        id: jumpSecondComboBox
                                        Layout.column: 1
                                        model: ListModel {
                                            ListElement {
                                                text: "0"
                                            }
                                            ListElement {
                                                text: "5"
                                            }
                                            ListElement {
                                                text: "10"
                                            }
                                            ListElement {
                                                text: "15"
                                            }
                                            ListElement {
                                                text: "20"
                                            }
                                            ListElement {
                                                text: "25"
                                            }
                                            ListElement {
                                                text: "30"
                                            }
                                        }

                                        onActivated: {
                                            localStorage.setJumpSecond(_page.jumpSeconds[index]);
                                        }
                                    }
                                }

                                Text {
                                    width: optionsPopup.width - 20
                                    font.pointSize: 10
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
                                    font.pointSize: 10
                                    text: "Автопереход в режим поверх всех окон"
                                }

                                Switch {
                                    id: autoTopMost
                                    onCheckedChanged: {
                                        localStorage.setAutoTopMost(checked);
                                        if (!checked) windowSettings.unsetStayOnTop()
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
                            switch (player.fillMode) {
                                case VideoOutput.PreserveAspectFit:
                                    player.fillMode = VideoOutput.PreserveAspectCrop;
                                    break;
                                case VideoOutput.PreserveAspectCrop:
                                    player.fillMode = VideoOutput.PreserveAspectFit;
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

        setSerieScrollPosition();
    }

    function nextVideo() {
        if (_page.selectedVideo === _page.releaseVideos.length) return;
        _page.restorePosition = 0;

        _page.selectedVideo++;
        const video = _page.releaseVideos[_page.selectedVideo];
        _page.isFullHdAllowed = "fullhd" in video;

        _page.videoSource = checkExistingVideoQuality();

        setSerieScrollPosition();
    }

    function setSerieScrollPosition() {
        let newPosition = _page.selectedVideo * 40 - serieScrollContainer.height;
        newPosition += 40;
        if (newPosition < 0) newPosition = 0;
        serieScrollContainer.contentY = newPosition;
    }

    function toggleFullScreen() {
        isFullScreen = !isFullScreen;
        changeFullScreenMode(isFullScreen);
    }

    function disableFullScreen() {
        if (!isFullScreen) return;
        isFullScreen = false;
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

    function jumpInPlayer(direction){
        const minutes = _page.jumpMinutes[jumpMinuteComboBox.currentIndex];
        const seconds = _page.jumpSeconds[jumpSecondComboBox.currentIndex];
        const jumpvalue = (minutes * 60 + seconds) * 1000;
        let seekPosition = player.position + (direction ? -jumpvalue : jumpvalue);
        if (seekPosition < 0) seekPosition = 80;
        if (seekPosition > player.duration) seekPosition = player.duration - 100;
        player.seek(seekPosition);
    }

    Component.onCompleted: {
        volumeSlider.value = player.volume * 100;
    }

}

