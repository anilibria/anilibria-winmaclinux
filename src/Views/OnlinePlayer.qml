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
import QtGraphicalEffects 1.0
import "../Controls"
import "../Theme"

Page {
    id: _page
    property var seenVideo: ({})
    property var seenMarks: ({})
    property bool isFromNavigated: false

    signal navigateFrom()
    signal setReleaseVideo()
    signal setCinemahallVideo()
    signal navigateTo()
    signal returnToReleasesPage()
    signal windowNotActived()
    signal playInPlayer()
    signal stopInPlayer()

    onPlayInPlayer: {
        player.play();
    }

    onStopInPlayer: {
        player.stop();
    }

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
            onlinePlayerViewModel.nextVideo();
            event.accepted = true;
        }
        if (event.key === Qt.Key_PageDown) {
            _page.previousVideo();
            event.accepted = true;
        }
        if (event.key === Qt.Key_Escape) {
            window.showPage("release");
        }
        if (event.key === Qt.Key_F11 || event.key === Qt.Key_F || event.key === 1040) {
            onlinePlayerViewModel.toggleFullScreen();
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
        if (event.key === Qt.Key_Home && !autoTopMost.checked) windowSettings.setStayOnTop();
        if (event.key === Qt.Key_End && !autoTopMost.checked) windowSettings.unsetStayOnTop();
    }

    onWindowNotActived: {
        if (!playerTimer.running) playerTimer.restart();
    }

    onNavigateFrom: {
        windowSettings.unsetStayOnTop();
        player.pause();
        onlinePlayerViewModel.isFullScreen = false;
    }

    onNavigateTo: {
        _page.isFromNavigated = true;
        const userSettings = JSON.parse(localStorage.getUserSettings());
        player.volume = userSettings.volume;
        autoNextVideo.checked = userSettings.autoNextVideo;
        autoTopMost.checked = userSettings.autoTopMost;
        jumpMinuteComboBox.currentIndex = onlinePlayerViewModel.jumpMinutes.indexOf(userSettings.jumpMinute);
        jumpSecondComboBox.currentIndex = onlinePlayerViewModel.jumpSeconds.indexOf(userSettings.jumpSecond);
        showReleaseInfo.checked = userSettings.showReleaseInfo;
        sendVolumeToRemoteSwitch.checked = applicationSettings.sendVolumeToRemote;
        onlinePlayerViewModel.sendPlaybackToRemoteSwitch = sendVolumeToRemoteSwitch.checked;
        sendPlaybackToRemoteSwitch.checked = applicationSettings.sendPlaybackToRemote;
        onlinePlayerViewModel.remotePlayer.port = applicationSettings.remotePort;
        remotePlayerPortComboBox.currentIndex = onlinePlayerViewModel.ports.indexOf(applicationSettings.remotePort);

        if (autoTopMost.checked && player.playbackState === MediaPlayer.PlayingState) windowSettings.setStayOnTop();
        switch (userSettings.quality) {
            case 0:
                onlinePlayerViewModel.videoQuality = "sd";
                break;
            case 1:
                onlinePlayerViewModel.videoQuality = "hd";
                break;
            case 2:
                onlinePlayerViewModel.videoQuality = "fullhd";
                break;
        }

        if (!onlinePlayerViewModel.navigateReleaseId && !onlinePlayerViewModel.isCinemahall) {
            const lastSeenReleaseId = onlinePlayerViewModel.getLastVideoSeen();
            if (lastSeenReleaseId === 0) return;

            const release = JSON.parse(localStorage.getRelease(lastSeenReleaseId));
            
            onlinePlayerViewModel.customPlaylistPosition = -1;
            onlinePlayerViewModel.navigateReleaseId = lastSeenReleaseId;
            onlinePlayerViewModel.navigateVideos = release.videos;
            onlinePlayerViewModel.navigatePoster = release.poster;

            onlinePlayerViewModel.setupForSingleRelease();
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
            onlinePlayerViewModel.toggleFullScreen();
        }
        onPositionChanged: {
            if (!(player.playbackState === MediaPlayer.PlayingState)) {
                if (controlPanel.opacity === 0) _page.setControlVisible(true);
                return;
            }

            _page.setControlVisible(true);
            const x = mouse.x;
            const y = mouse.y;
            onlinePlayerViewModel.lastMouseYPosition = y;
            if (y > _page.height - controlPanel.height) {
                playerTimer.stop();
            } else {
                playerTimer.restart();
            }
        }
        onExited: {
            if (_page.height - onlinePlayerViewModel.lastMouseYPosition < 10) if (!playerTimer.running) playerTimer.restart();
        }
    }

    QtPlayer {
        id: player
        anchors.fill: parent
        source: onlinePlayerViewModel.videoSource
        playbackRate: onlinePlayerViewModel.playbackRate
        onPlaybackStateChanged: {
            onlinePlayerViewModel.playerPlaybackState = playbackState;
            if (playbackState === MediaPlayer.PlayingState && autoTopMost.checked) {
                windowSettings.setStayOnTop();
            } else {
                if (autoTopMost.checked) windowSettings.unsetStayOnTop();
            }

            releasePosterArea.visible = showReleaseInfo.checked && playbackState !== MediaPlayer.PlayingState;
            playButton.visible = playbackState === MediaPlayer.PausedState || playbackState === MediaPlayer.StoppedState;
            pauseButton.visible = playbackState === MediaPlayer.PlayingState;
            if (playbackState === MediaPlayer.PlayingState) {
                playerTimer.start();
            } else {
                playerTimer.stop();
                _page.setControlVisible(true);
            }

            if (!sendPlaybackToRemoteSwitch.checked) return;

            if (playbackState === MediaPlayer.PlayingState) onlinePlayerViewModel.broadcastPlaybackState("play");
            if (playbackState === MediaPlayer.PausedState) onlinePlayerViewModel.broadcastPlaybackState("pause");
        }
        onVolumeChanged: {
            volumeSlider.value = volume * 100;
            onlinePlayerViewModel.volumeSlider = volumeSlider.value;
            if (applicationSettings.sendVolumeToRemote) onlinePlayerViewModel.broadcastVolume(volumeSlider.value);
        }
        onStatusChanged: {
            if (status === MediaPlayer.Loading) onlinePlayerViewModel.isBuffering = true;

            if (status === MediaPlayer.EndOfMedia && autoNextVideo.checked) onlinePlayerViewModel.nextVideo();

            if (status === MediaPlayer.InvalidMedia) {
                console.log("InvalidMedia")
            }

            if (status === MediaPlayer.Buffering) onlinePlayerViewModel.isBuffering = true;

            if (status === MediaPlayer.Buffered) {                
                onlinePlayerViewModel.isBuffering = false;
                if (onlinePlayerViewModel.restorePosition > 0){
                    player.seek(onlinePlayerViewModel.restorePosition);
                    if (player.position >= onlinePlayerViewModel.restorePosition) onlinePlayerViewModel.restorePosition = 0;
                } else {
                    if (_page.isFromNavigated) {
                        const videoPosition = onlinePlayerViewModel.getCurrentVideoSeenVideoPosition()
                        if (videoPosition > 0) player.seek(videoPosition);
                        _page.isFromNavigated = false;
                    }
                }
            }
        }

        onPositionChanged: {
            if (!playerLocation.pressed && onlinePlayerViewModel.lastMovedPosition === 0) playerLocation.value = position;

            onlinePlayerViewModel.changeVideoPosition(duration, position);

            if (onlinePlayerViewModel.positionIterator < 20) onlinePlayerViewModel.positionIterator++;

            if (onlinePlayerViewModel.positionIterator >= 20) {
                onlinePlayerViewModel.positionIterator = 0;
                onlinePlayerViewModel.setVideoSeens(onlinePlayerViewModel.selectedRelease, onlinePlayerViewModel.selectedVideo, position);
            }

            if (!(onlinePlayerViewModel.selectedRelease in _page.seenMarks && onlinePlayerViewModel.selectedVideo in _page.seenMarks[onlinePlayerViewModel.selectedRelease])) {
                if (duration > 0 && position > 0) {
                    const positionPercent = position / duration * 100;
                    if (positionPercent >= 90) {
                        let seenMarks = getReleaseSeens(onlinePlayerViewModel.selectedRelease);
                        seenMarks[onlinePlayerViewModel.selectedVideo] = true;
                        const obj = _page.seenMarks;
                        _page.seenMarks = obj;
                        onlinePlayerViewModel.setSeenMark(onlinePlayerViewModel.selectedRelease, onlinePlayerViewModel.selectedVideo, true);
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
                    model: onlinePlayerViewModel.videos
                    delegate: Row {
                        Rectangle {
                            height: isGroup ? 70 : 40
                            width: seriesPopup.width
                            color: selectedVideo ? ApplicationTheme.playlistSelectedBackground : ApplicationTheme.playlistBackground
                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                onEntered: {
                                    if (playerTimer.running) playerTimer.stop();
                                }
                                onClicked: {
                                    if (isGroup) return;

                                    onlinePlayerViewModel.selectVideo(releaseId, order);
                                }
                            }
                            Text {
                                visible: !isGroup
                                color: selectedVideo ? ApplicationTheme.playlistSelectedText : ApplicationTheme.playlistText
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                text: title
                            }

                            Rectangle {
                                color: "transparent"
                                anchors.fill: parent
                                anchors.leftMargin: 10
                                anchors.rightMargin: 10

                                Text {
                                    visible: isGroup
                                    color: selectedVideo ? ApplicationTheme.playlistSelectedText : ApplicationTheme.playlistText
                                    text: title
                                    width: parent.width
                                    anchors.verticalCenter: parent.verticalCenter
                                    maximumLineCount: 3
                                    font.bold: true
                                    elide: Text.ElideRight
                                    wrapMode: Text.WordWrap
                                }
                            }

                            Row {
                                width: 72
                                spacing: 0
                                anchors.right: parent.right
                                anchors.rightMargin: 18

                                IconButton {
                                    height: 36
                                    width: 36
                                    visible: !isGroup
                                    iconColor: _page.seenMarks && releaseId in _page.seenMarks && order in _page.seenMarks[releaseId] ? ApplicationTheme.filterIconButtonGreenColor : ApplicationTheme.filterIconButtonColor
                                    hoverColor: ApplicationTheme.filterIconButtonHoverColor
                                    iconPath: _page.seenMarks && releaseId in _page.seenMarks && order in _page.seenMarks[releaseId] ? "../Assets/Icons/seenmarkselected.svg" : "../Assets/Icons/seenmark.svg"
                                    iconWidth: 22
                                    iconHeight: 22
                                    onButtonHoverEnter: {
                                        if (playerTimer.running) playerTimer.stop();
                                    }
                                    onButtonPressed: {
                                        let newState = false;
                                        let seenMarks = getReleaseSeens(releaseId);

                                        if (seenMarks[order]) {
                                            delete seenMarks[order];
                                            newState = false;
                                        } else {
                                            seenMarks[order] = true;
                                            newState = true;

                                        }

                                        const oldSeenMarks = _page.seenMarks;
                                        _page.seenMarks = {};
                                        _page.seenMarks = oldSeenMarks;

                                        onlinePlayerViewModel.setSeenMark(releaseId, order, newState);
                                    }
                                }

                                IconButton {
                                    height: 36
                                    width: 36
                                    visible: !isGroup
                                    iconColor: ApplicationTheme.filterIconButtonColor
                                    hoverColor: ApplicationTheme.filterIconButtonHoverColor
                                    iconPath: "../Assets/Icons/download.svg"
                                    iconWidth: 22
                                    iconHeight: 22
                                    onButtonHoverEnter: {
                                        if (playerTimer.running) playerTimer.stop();
                                    }
                                    onButtonPressed: {
                                        switch (onlinePlayerViewModel.videoQuality) {
                                            case "fullhd":
                                            case "hd":
                                                Qt.openUrlExternally(hdfile);
                                                break;
                                            case "sd":
                                                Qt.openUrlExternally(sdfile);
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
        color: ApplicationTheme.playerControlBackground
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
                    if (!pressed && onlinePlayerViewModel.lastMovedPosition > 0) {
                        player.seek(onlinePlayerViewModel.lastMovedPosition);
                        onlinePlayerViewModel.broadcastVideoPosition(onlinePlayerViewModel.lastMovedPosition.toString() + `/` + player.duration.toString());
                        onlinePlayerViewModel.lastMovedPosition = 0;
                    }
                    controlPanel.forceActiveFocus();
                }

                onMoved: {
                    if (pressed) onlinePlayerViewModel.lastMovedPosition = value;
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

                PlainText {
                    text: onlinePlayerViewModel.displayVideoPosition
                    fontPointSize: 10
                }

                Row {
                    height: 20
                    anchors.horizontalCenter: parent.horizontalCenter
                    ToggleButton {
                        height: 20
                        width: 60
                        text: "1080p"
                        visible: player.duration > 0 && onlinePlayerViewModel.isFullHdAllowed
                        isChecked: onlinePlayerViewModel.videoQuality === `fullhd`
                        onButtonClicked: {
                            onlinePlayerViewModel.restorePosition = player.position;
                            onlinePlayerViewModel.changeVideoQuality(`fullhd`);
                            localStorage.setVideoQuality(2);
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 60
                        visible: player.duration > 0
                        text: "720p"
                        isChecked: onlinePlayerViewModel.videoQuality === `hd`
                        onButtonClicked: {
                            onlinePlayerViewModel.restorePosition = player.position;
                            onlinePlayerViewModel.changeVideoQuality(`hd`);
                            localStorage.setVideoQuality(1);
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 60
                        visible: player.duration > 0
                        text: "480p"
                        isChecked: onlinePlayerViewModel.videoQuality === `sd`
                        onButtonClicked: {
                            onlinePlayerViewModel.restorePosition = player.position;
                            onlinePlayerViewModel.changeVideoQuality(`sd`);
                            localStorage.setVideoQuality(0);
                        }
                    }
                    Rectangle {
                        width: 20
                        height: 20
                        visible: player.duration > 0
                        color: "transparent"
                        PlainText {
                            anchors.centerIn: parent
                            text: "|"
                            fontPointSize: 10
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x0.25"
                        isChecked: onlinePlayerViewModel.playbackRate === 0.25
                        onButtonClicked: {
                            onlinePlayerViewModel.setVideoSpeed(0.25)
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x0.5"
                        isChecked: onlinePlayerViewModel.playbackRate === 0.5
                        onButtonClicked: {
                            onlinePlayerViewModel.setVideoSpeed(0.5);
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x0.75"
                        isChecked: onlinePlayerViewModel.playbackRate === 0.75
                        onButtonClicked: {
                            onlinePlayerViewModel.setVideoSpeed(0.75);
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x1"
                        isChecked: onlinePlayerViewModel.playbackRate === 1
                        onButtonClicked: {
                            onlinePlayerViewModel.setVideoSpeed(1);
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x1.25"
                        isChecked: onlinePlayerViewModel.playbackRate === 1.1
                        onButtonClicked: {
                            onlinePlayerViewModel.setVideoSpeed(1.1);
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x1.5"
                        isChecked: onlinePlayerViewModel.playbackRate === 1.2
                        onButtonClicked: {
                            onlinePlayerViewModel.setVideoSpeed(1.2);
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x1.75"
                        isChecked: onlinePlayerViewModel.playbackRate === 1.3
                        onButtonClicked: {
                            onlinePlayerViewModel.setVideoSpeed(1.3);
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x2"
                        isChecked: onlinePlayerViewModel.playbackRate === 1.5
                        onButtonClicked: {
                            onlinePlayerViewModel.setVideoSpeed(1.5);
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x3"
                        isChecked: onlinePlayerViewModel.playbackRate === 2
                        onButtonClicked: {
                            onlinePlayerViewModel.setVideoSpeed(2);
                        }
                    }
                }

                PlainText {
                    height: 20
                    anchors.right: parent.right
                    anchors.rightMargin: 4
                    text: onlinePlayerViewModel.displayEndVideoPosition
                    fontPointSize: 10
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
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
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
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
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
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
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
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
                        iconPath: "../Assets/Icons/step-backward.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            onlinePlayerViewModel.previousVideo();
                        }
                    }
                    IconButton {
                        id: playButton
                        visible: false
                        width: 40
                        height: 40
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
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
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
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
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
                        iconPath: "../Assets/Icons/step-forward.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            onlinePlayerViewModel.nextVideo();
                        }
                    }
                    IconButton {
                        width: 40
                        height: 40
                        visible: player.duration > 0
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
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
                        id: topmostButton
                        width: 40
                        height: 40
                        visible: !autoTopMost.checked
                        iconColor: windowSettings.isTopMost ? ApplicationTheme.filterIconButtonGreenColor : ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
                        iconPath: "../Assets/Icons/topmostwindow.svg"
                        iconWidth: 29
                        iconHeight: 29
                        onButtonPressed: {
                            windowSettings.toggleStayOnTopMode();
                        }

                        ToolTip.delay: 1000
                        ToolTip.visible: topmostButton.hovered
                        ToolTip.text: windowSettings.isTopMost ? "Выключить режим поверх всех окон" : "Включить режим поверх всех окон"
                    }

                    IconButton {
                        id: remotePlayerButton
                        width: 40
                        height: 40
                        iconColor: onlinePlayerViewModel.remotePlayer.started ? ApplicationTheme.filterIconButtonGreenColor : ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
                        iconPath: "../Assets/Icons/connect.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            remotePlayerPopup.open();
                        }

                        ToolTip.delay: 1000
                        ToolTip.visible: remotePlayerButton.hovered
                        ToolTip.text: onlinePlayerViewModel.remotePlayer.started ? "Удаленный плеер включен" : "Удаленный плеер выключен, откройте настройки для подключения"

                        Popup {
                            id: remotePlayerPopup
                            x: optionsButton.width - 300
                            y: optionsButton.height - 380
                            width: 300
                            height: 380

                            modal: true
                            focus: true
                            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                            Column {
                                width: parent.width
                                spacing: 10

                                PlainText {
                                    width: optionsPopup.width - 20
                                    fontPointSize: 10
                                    text: "Включить удаленный плеер"
                                }

                                Switch {
                                    id: stateRemotePlayer
                                    onCheckedChanged: {
                                        if (checked) {
                                            onlinePlayerViewModel.remotePlayer.port = applicationSettings.remotePort;
                                            onlinePlayerViewModel.remotePlayer.startServer();
                                        } else {
                                            onlinePlayerViewModel.remotePlayer.stopServer();
                                        }
                                    }
                                }

                                PlainText {
                                    width: optionsPopup.width - 20
                                    fontPointSize: 10
                                    text: "Порт"
                                }

                                CommonComboBox {
                                    id: remotePlayerPortComboBox
                                    enabled: !stateRemotePlayer.checked
                                    Layout.column: 0
                                    model: ListModel {
                                        ListElement {
                                            text: "12345"
                                        }
                                        ListElement {
                                            text: "34560"
                                        }
                                        ListElement {
                                            text: "52354"
                                        }
                                        ListElement {
                                            text: "67289"
                                        }
                                    }
                                    onActivated: {
                                        applicationSettings.remotePort = _page.ports[index];
                                    }
                                }

                                PlainText {
                                    width: optionsPopup.width - 20
                                    fontPointSize: 10
                                    text: "Передавать громкость"
                                }

                                Switch {
                                    id: sendVolumeToRemoteSwitch
                                    onCheckedChanged: {
                                        applicationSettings.sendVolumeToRemote = checked;

                                        onlinePlayerViewModel.sendVolumeToRemote = checked;
                                    }
                                }

                                PlainText {
                                    width: optionsPopup.width - 20
                                    fontPointSize: 10
                                    text: "Передавать воспроизведение/пауза"
                                }

                                Switch {
                                    id: sendPlaybackToRemoteSwitch
                                    onCheckedChanged: {
                                        applicationSettings.sendPlaybackToRemote = checked;

                                        onlinePlayerViewModel.sendPlaybackToRemoteSwitch = checked;
                                    }
                                }

                                PlainText {
                                    width: optionsPopup.width - 20
                                    fontPointSize: 10
                                    text: "Подключено: " + onlinePlayerViewModel.remotePlayer.countUsers
                                }
                            }
                        }

                    }

                    IconButton {
                        id: optionsButton
                        width: 40
                        height: 40
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
                        iconPath: "../Assets/Icons/options.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            optionsPopup.open();
                        }

                        Popup {
                            id: optionsPopup
                            x: optionsButton.width - 300
                            y: optionsButton.height - 340
                            width: 300
                            height: 340

                            modal: true
                            focus: true
                            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                            Column {
                                width: parent.width
                                spacing: 10
                                PlainText {
                                    width: optionsPopup.width - 20
                                    fontPointSize: 10
                                    text: "Время прыжка"
                                }

                                Grid {
                                    columns: 2
                                    spacing: 2

                                    CommonComboBox {
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
                                            localStorage.setJumpMinute(onlinePlayerViewModel.jumpMinutes[index]);
                                        }
                                    }
                                    CommonComboBox {
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
                                            localStorage.setJumpSecond(onlinePlayerViewModel.jumpSeconds[index]);
                                        }
                                    }
                                }

                                PlainText {
                                    width: optionsPopup.width - 20
                                    fontPointSize: 10
                                    text: "Автопереход между сериями"
                                }

                                Switch {
                                    id: autoNextVideo
                                    onCheckedChanged: {
                                        localStorage.setAutoNextVideo(checked);
                                    }
                                }

                                PlainText {
                                    width: optionsPopup.width - 20
                                    fontPointSize: 10
                                    text: "Автопереход в режим поверх всех окон"
                                }

                                Switch {
                                    id: autoTopMost
                                    onCheckedChanged: {
                                        localStorage.setAutoTopMost(checked);
                                        if (!checked) windowSettings.unsetStayOnTop()
                                    }
                                }

                                PlainText {
                                    width: optionsPopup.width - 20
                                    fontPointSize: 10
                                    text: "Информация о релизе"
                                }

                                Switch {
                                    id: showReleaseInfo
                                    onCheckedChanged: {
                                        localStorage.setShowReleaseInfo(checked);
                                        if (!checked) releasePosterArea.visible = false;
                                    }
                                }
                            }
                        }

                        ToolTip.delay: 1000
                        ToolTip.visible: optionsButton.hovered
                        ToolTip.text: "Показать настройки страницы Видеоплеер"

                    }
                    IconButton {
                        id: resizeVideoButton
                        width: 40
                        height: 40
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
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

                        ToolTip.delay: 1000
                        ToolTip.visible: resizeVideoButton.hovered
                        ToolTip.text: "Включить режим обрезки видео потока, работает только на Linux и macOS"
                    }
                    IconButton {
                        id: fullScreenButton
                        width: 40
                        height: 40
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
                        iconPath: "../Assets/Icons/fullscreen.svg"
                        iconWidth: 29
                        iconHeight: 29
                        onButtonPressed: {
                            onlinePlayerViewModel.toggleFullScreen();
                        }

                        ToolTip.delay: 1000
                        ToolTip.visible: fullScreenButton.hovered
                        ToolTip.text: "Переключиться между полноэкранным и оконным режимами"
                    }
                }
            }
        }

        Behavior on opacity {
            NumberAnimation { duration: 200 }
        }
    }

    Rectangle {
        id: mask
        width: 180
        height: 260
        radius: 10
        visible: false
    }

    Rectangle {
        id: releasePosterArea
        anchors.right: parent.right
        anchors.top: parent.top
        width: 182
        height: 272
        border.color: "#adadad"
        border.width: 1
        radius: 12
        Image {
            anchors.centerIn: parent
            source: onlinePlayerViewModel.releasePoster
            fillMode: Image.PreserveAspectCrop
            width: 180
            height: 270
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: mask
            }
        }
    }

    Rectangle {
        width: 80
        height: 80
        color: "white"
        radius: 20
        opacity: 0.8
        visible: onlinePlayerViewModel.isBuffering
        anchors.centerIn: parent
        AnimatedImage {
            id: spinner
            anchors.centerIn: parent
            paused: !_page.visible
            playing: _page.visible
            source: _page.visible ? "../Assets/Icons/spinner.gif" : ""
        }
    }

    function getReleaseSeens(releaseId) {
        let seenMarks = [];
        if (releaseId in _page.seenMarks) {
            seenMarks = _page.seenMarks[releaseId];
        } else {
            _page.seenMarks[releaseId] = seenMarks;
        }

        return seenMarks;
    }

    function setSerieScrollPosition() {
        let newPosition = onlinePlayerViewModel.selectedVideo * 40 - serieScrollContainer.height;
        newPosition += 40;
        if (newPosition < 0) newPosition = 0;
        serieScrollContainer.contentY = newPosition;
    }

    function refreshSeenMarks() {
        let releaseIds = [];

        if (onlinePlayerViewModel.isCinemahall) {
            releaseIds = JSON.parse(localStorage.getCinemahallReleases()).map(a => a.id);
        } else {
            releaseIds.push(onlinePlayerViewModel.selectedRelease);
        }

        _page.seenMarks = JSON.parse(onlinePlayerViewModel.getReleasesSeenMarks(releaseIds));
    }

    function jumpInPlayer(direction){
        const minutes = onlinePlayerViewModel.jumpMinutes[jumpMinuteComboBox.currentIndex];
        const seconds = onlinePlayerViewModel.jumpSeconds[jumpSecondComboBox.currentIndex];
        const jumpvalue = (minutes * 60 + seconds) * 1000;
        let seekPosition = player.position + (direction ? -jumpvalue : jumpvalue);
        if (seekPosition < 0) seekPosition = 80;
        if (seekPosition > player.duration) seekPosition = player.duration - 100;
        player.seek(seekPosition);
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

    Component.onCompleted: {
        volumeSlider.value = player.volume * 100;
    }

}

