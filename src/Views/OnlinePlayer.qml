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
import "Videoplayer"

Page {
    id: _page
    property var seenVideo: ({})
    property var seenMarks: ({})
    property var videoPlayerSource
    property var videoOutputSource
    property var lastPlayerPosition

    signal navigateFrom()
    signal setReleaseVideo()
    signal setCinemahallVideo()
    signal navigateTo()
    signal returnToReleasesPage()
    signal windowNotActived()
    signal playInPlayer()
    signal stopInPlayer()
    signal playerCreated()

    onPlayInPlayer: {
        playerLoader.item.play();
    }

    onStopInPlayer: {
        playerLoader.item.stop();
    }

    function togglePlayback() {
        if (playerLoader.item.playbackState === MediaPlayer.PlayingState) {
            playerLoader.item.pause();
            return;
        }
        if (playerLoader.item.playbackState === MediaPlayer.PausedState || playerLoader.item.playbackState === MediaPlayer.StoppedState) {
            playerLoader.item.play();
        }
    }

    Keys.onSpacePressed: {
        togglePlayback();
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_PageDown || event.key === Qt.Key_N || event.key === 1058 || event.key === Qt.Key_MediaNext) {
            onlinePlayerViewModel.nextVideo();
            event.accepted = true;
        }
        if (event.key === Qt.Key_PageUp || event.key === Qt.Key_P || event.key === 1047 || event.key === Qt.Key_MediaPrevious) {
            onlinePlayerViewModel.previousVideo();
            event.accepted = true;
        }
        if (event.key === Qt.Key_Escape) {
            mainViewModel.selectPage(userConfigurationViewModel.startPage);
        }
        if (event.key === Qt.Key_F11 || event.key === Qt.Key_F || event.key === 1040) {
            onlinePlayerViewModel.toggleFullScreen();
        }
        if (event.key === Qt.Key_Up || event.key === Qt.Key_VolumeUp) {
            if (playerLoader.item.volume < 1) playerLoader.item.volume += .1;
            if (playerLoader.item.volume > 1) playerLoader.item.volume = 1;

            volumeSlider.value = playerLoader.item.volume * 100;
        }
        if (event.key === Qt.Key_Down || event.key === Qt.Key_VolumeDown) {
            let newVolume = playerLoader.item.volume;
            if (newVolume > 0) newVolume -= .1;
            if (newVolume < 0) newVolume = 0;

            playerLoader.item.volume = newVolume;

            volumeSlider.value = playerLoader.item.volume * 100;
        }
        if (event.key === Qt.Key_M || event.key === 1068 || event.key === Qt.Key_VolumeMute) playerLoader.item.muted = !playerLoader.item.muted;
        if ((event.key === Qt.Key_T || event.key === 1045) && !autoTopMost.checked) windowSettings.toggleStayOnTopMode();
        if (event.key === Qt.Key_Left) playerLoader.item.seek(onlinePlayerViewModel.jumpInPlayer(jumpMinuteComboBox.currentIndex, jumpSecondComboBox.currentIndex, true));
        if (event.key === Qt.Key_Right) playerLoader.item.seek(onlinePlayerViewModel.jumpInPlayer(jumpMinuteComboBox.currentIndex, jumpSecondComboBox.currentIndex, false));
        if (event.key === Qt.Key_Home && !autoTopMost.checked) windowSettings.setStayOnTop();
        if (event.key === Qt.Key_End && !autoTopMost.checked) windowSettings.unsetStayOnTop();
        if (event.key === Qt.Key_Play) playerLoader.item.play();
        if (event.key === Qt.Key_MediaPause) playerLoader.item.pause();
        if (event.key === Qt.Key_MediaTogglePlayPause) togglePlayback();
    }

    onWindowNotActived: {
        if (!playerTimer.running) playerTimer.restart();
    }

    onNavigateFrom: {
        windowSettings.unsetStayOnTop();
        onlinePlayerViewModel.isFullScreen = false;
        const enableVideoPreview = !onlinePlayerWindowViewModel.isStandartPlayer || (onlinePlayerWindowViewModel.isStandartPlayer && onlinePlayerWindowViewModel.isQt515);
        if (enableVideoPreview && playerLoader.item.playbackState === MediaPlayer.PlayingState && showVideoPreview.checked) {
            onlinePlayerWindow.showWindow();
        } else {
            playerLoader.item.pause();
        }
    }

    onNavigateTo: {
        _page.forceActiveFocus();
        if (onlinePlayerWindowViewModel.opened) onlinePlayerWindow.hideWindow(false);
        onlinePlayerViewModel.isFromNavigated = true;
        const userSettings = JSON.parse(localStorage.getUserSettings());
        playerLoader.item.volume = userSettings.volume;
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
        showVideoPreview.checked = userSettings.showVideoPreview;

        if (autoTopMost.checked && playerLoader.item.playbackState === MediaPlayer.PlayingState) windowSettings.setStayOnTop();
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

            onlinePlayerViewModel.quickSetupForSingleRelease(lastSeenReleaseId);
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
        onClicked: {
            if (playerLoader.item.playbackState === MediaPlayer.PlayingState) {
                if (controlPanel.opacity !== 1)playerTimer.restart();
                _page.setControlVisible(!(controlPanel.opacity === 1));
            }
        }
        onDoubleClicked: {
            onlinePlayerViewModel.toggleFullScreen();
        }
        onPositionChanged: {
            if (!(playerLoader.item.playbackState === MediaPlayer.PlayingState)) {
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

    Loader {
        id: playerLoader
        anchors.fill: parent
        source: onlinePlayerWindowViewModel.playerComponent
        onLoaded: {
            if (playerLoader.source) {
                _page.videoPlayerSource = playerLoader.item.videoPlayerSource;
                _page.videoOutputSource = playerLoader.item.videoOutputSource;

                playerCreated();

                playerLoader.item.source = Qt.binding(function() { return onlinePlayerViewModel.videoSource; });
                playerLoader.item.playbackRate = Qt.binding(function() { return onlinePlayerViewModel.playbackRate; });

                playerLoader.item.playbackStateChanged.connect(loaderPlaybackStateChanged);
                playerLoader.item.volumeChanged.connect(loaderVolumeChanged);
                playerLoader.item.statusChanged.connect(loaderStatusChanged);
                playerLoader.item.positionChanged.connect(loaderPositionChanged);

                const loadedVolumeState = onlinePlayerViewModel.volumeSlider / 100;
                if (loadedVolumeState >= 0) playerLoader.item.volume = loadedVolumeState;
                playerLoader.item.muted = onlinePlayerViewModel.muted;
                if (userConfigurationViewModel.isCroppedPlayer) playerLoader.item.fillMode = VideoOutput.PreserveAspectCrop;
            }
        }

        function loaderPlaybackStateChanged() {
            onlinePlayerViewModel.playerPlaybackState = playerLoader.item.playbackState;
            const playbackState = onlinePlayerViewModel.playerPlaybackState;
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

        function loaderVolumeChanged() {
            volumeSlider.value = playerLoader.item.volume * 100;
            onlinePlayerViewModel.volumeSlider = volumeSlider.value;
            if (applicationSettings.sendVolumeToRemote) onlinePlayerViewModel.broadcastVolume(onlinePlayerViewModel.volumeSlider);
        }

        function loaderStatusChanged() {
            const status = playerLoader.item.status;
            if (status === MediaPlayer.Loading) onlinePlayerViewModel.isBuffering = true;

            if (status === MediaPlayer.EndOfMedia && autoNextVideo.checked) onlinePlayerViewModel.nextVideo();

            if (status === MediaPlayer.InvalidMedia) {
                console.log("InvalidMedia")
            }

            if (status === MediaPlayer.Buffering) onlinePlayerViewModel.isBuffering = true;

            if (status === MediaPlayer.Buffered) {
                onlinePlayerViewModel.isBuffering = false;
                if (onlinePlayerViewModel.restorePosition > 0){
                    playerLoader.item.seek(onlinePlayerViewModel.restorePosition);
                    console.log('playerLoader.item.seek(onlinePlayerViewModel.restorePosition);')
                    if (playerLoader.item.position >= onlinePlayerViewModel.restorePosition) onlinePlayerViewModel.restorePosition = 0;
                } else {
                    if (onlinePlayerViewModel.isFromNavigated) {
                        onlinePlayerViewModel.isFromNavigated = false;

                        const videoPosition = onlinePlayerViewModel.getCurrentVideoSeenVideoPosition();
                        if (videoPosition > 0) {
                            playerLoader.item.seek(videoPosition);
                            console.log('playerLoader.item.seek(videoPosition);')
                        }
                    }
                }
            }
        }

        function loaderPositionChanged() {
            const position = playerLoader.item.position;
            const duration = playerLoader.item.duration;
            const playBackState = playerLoader.item.playbackState;
            const status = playerLoader.item.status;

            if (!playerLocation.pressed && onlinePlayerViewModel.lastMovedPosition === 0) playerLocation.value = position;

            onlinePlayerViewModel.changeVideoPosition(duration, position);

            if (onlinePlayerViewModel.positionIterator < 20 && playBackState === MediaPlayer.PlayingState && status === MediaPlayer.Buffered) onlinePlayerViewModel.positionIterator++;

            if (onlinePlayerViewModel.positionIterator >= 20) {
                onlinePlayerViewModel.positionIterator = 0;
                onlinePlayerViewModel.setVideoSeens(onlinePlayerViewModel.selectedRelease, onlinePlayerViewModel.selectedVideo, position);
            }

            if (!releasesViewModel.getSeriaSeenMark(onlinePlayerViewModel.selectedRelease, onlinePlayerViewModel.selectedVideo)) {
                if (duration > 0 && position > 0) {
                    const positionPercent = position / duration * 100;
                    if (positionPercent >= 90 && !onlinePlayerViewModel.seenMarkedAtEnd) {
                        releasesViewModel.setSeenMark(onlinePlayerViewModel.selectedRelease, onlinePlayerViewModel.selectedVideo, true);
                        onlinePlayerViewModel.seenMarkedAtEnd = true;
                        onlinePlayerViewModel.refreshSingleVideo(onlinePlayerViewModel.selectedRelease, onlinePlayerViewModel.selectedVideo);
                        releasesViewModel.items.refreshSingleItem(onlinePlayerViewModel.selectedRelease);
                        if (onlinePlayerViewModel.isLastSeriaIsSingleRelease()) {
                            const nextReleaseId = releaseLinkedSeries.getNextLinkedRelease(onlinePlayerViewModel.selectedRelease);
                            if (nextReleaseId > 0) {
                                onlinePlayerViewModel.showNextPosterRelease = true;
                                onlinePlayerViewModel.nextReleasePoster = releasesViewModel.getReleasePoster(nextReleaseId);
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: seriesPopup
        width: 230
        y: userConfigurationViewModel.fixedControlPanel ? fullHeight / 2 - (height / 2) : 0
        height: itemsContent.height > panelHeight ? panelHeight : itemsContent.height
        color: "transparent"

        property int fullHeight: _page.height - controlPanel.height
        property int panelHeight: userConfigurationViewModel.fixedControlPanel ? fullHeight / 2 : fullHeight - 20

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
                            color: selectedVideo ? applicationThemeViewModel.playlistSelectedBackground : applicationThemeViewModel.playlistBackground
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
                                color: selectedVideo ? applicationThemeViewModel.playlistSelectedText : applicationThemeViewModel.playlistText
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
                                    color: selectedVideo ? applicationThemeViewModel.playlistSelectedText : applicationThemeViewModel.playlistText
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
                                width: 62
                                spacing: 0
                                anchors.right: parent.right
                                anchors.rightMargin: 24

                                IconButton {
                                    height: 36
                                    width: 36
                                    visible: !isGroup
                                    hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                                    iconPath: isSeen ? applicationThemeViewModel.currentItems.iconPlayerSeen : applicationThemeViewModel.currentItems.iconPlayerUnseen
                                    iconWidth: 22
                                    iconHeight: 22
                                    onButtonPressed: {
                                        releasesViewModel.toggleSeenMark(releaseId, order);
                                        onlinePlayerViewModel.refreshSingleVideo(releaseId, order);
                                        releasesViewModel.items.refreshSingleItem(releaseId);
                                    }
                                }

                                IconButton {
                                    height: 36
                                    width: 36
                                    visible: !isGroup
                                    hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                                    iconPath: applicationThemeViewModel.currentItems.iconPlayerExternal
                                    iconWidth: 22
                                    iconHeight: 22
                                    onButtonPressed: {
                                        externalMenu.open();
                                    }

                                    CommonMenu {
                                        id: externalMenu
                                        y: parent.y
                                        width: 300

                                        CommonMenuItem {
                                            text: "Открыть в браузере"
                                            onPressed: {
                                                let video;
                                                switch (onlinePlayerViewModel.videoQuality) {
                                                    case "fullhd":
                                                        video = fullhd;
                                                        break;
                                                    case "hd":
                                                        video = hd;
                                                        break;
                                                    case "sd":
                                                        video = sd;
                                                        break;
                                                }
                                                if (!video) return;

                                                Qt.openUrlExternally("https://anilibria.github.io/anilibria-win/videotester.html?video=" + video.replace("https://", ""));

                                                externalMenu.close();
                                            }
                                        }
                                        CommonMenuItem {
                                            text: "Открыть во внешнем плеере"
                                            onPressed: {
                                                let video;
                                                switch (onlinePlayerViewModel.videoQuality) {
                                                    case "fullhd":
                                                        video = fullhd;
                                                        break;
                                                    case "hd":
                                                        video = hd;
                                                        break;
                                                    case "sd":
                                                        video = sd;
                                                        break;
                                                }
                                                if (!video) return;

                                                onlinePlayerViewModel.openVideoInExternalPlayer(video);

                                                externalMenu.close();
                                            }
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

    Item {
        id: controlPanel
        anchors.bottom: parent.bottom
        width: _page.width
        height: 100

        Rectangle {
            id: controlPanelBackground
            anchors.fill: parent
            color: applicationThemeViewModel.currentItems.playerControlBackground
        }

        Column {
            width: controlPanel.width
            height: 100

            Slider {
                id: playerLocation
                visible: onlinePlayerViewModel.videoDuration > 0
                height: 20
                width: controlPanel.width
                from: 1
                value: 1
                to: onlinePlayerViewModel.videoDuration
                onPressedChanged: {
                    if (!pressed && onlinePlayerViewModel.lastMovedPosition > 0) {
                        playerLoader.item.seek(onlinePlayerViewModel.lastMovedPosition);
                        console.log('playerLoader.item.seek(onlinePlayerViewModel.lastMovedPosition);');
                        onlinePlayerViewModel.lastMovedPosition = 0;
                        onlinePlayerViewModel.broadcastVideoPosition(onlinePlayerViewModel.lastMovedPosition.toString() + `/` + playerLoader.item.duration.toString());                        
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
                visible: onlinePlayerViewModel.videoDuration === 0
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
                        width: userConfigurationViewModel.hidedQuality && !isChecked ? 0 : 60
                        text: "1080p"
                        visible: onlinePlayerViewModel.videoDuration > 0 && onlinePlayerViewModel.isFullHdAllowed && !(userConfigurationViewModel.hidedQuality && !isChecked)
                        isChecked: onlinePlayerViewModel.videoQuality === `fullhd`
                        onButtonClicked: {
                            onlinePlayerViewModel.restorePosition = playerLoader.item.position;
                            onlinePlayerViewModel.changeVideoQuality(`fullhd`);
                            localStorage.setVideoQuality(2);
                        }
                        onButtonAlreadyClicked: {
                            userConfigurationViewModel.hidedQuality = !userConfigurationViewModel.hidedQuality;
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: userConfigurationViewModel.hidedQuality && !isChecked ? 0 : 60
                        visible: onlinePlayerViewModel.videoDuration > 0 && !(userConfigurationViewModel.hidedQuality && !isChecked)
                        text: "720p"
                        isChecked: onlinePlayerViewModel.videoQuality === `hd`
                        onButtonClicked: {
                            onlinePlayerViewModel.restorePosition = playerLoader.item.position;
                            onlinePlayerViewModel.changeVideoQuality(`hd`);
                            localStorage.setVideoQuality(1);
                        }
                        onButtonAlreadyClicked: {
                            userConfigurationViewModel.hidedQuality = !userConfigurationViewModel.hidedQuality;
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: userConfigurationViewModel.hidedQuality && !isChecked ? 0 : 60
                        visible: onlinePlayerViewModel.videoDuration > 0 && !(userConfigurationViewModel.hidedQuality && !isChecked)
                        text: "480p"
                        isChecked: onlinePlayerViewModel.videoQuality === `sd`
                        onButtonClicked: {
                            onlinePlayerViewModel.restorePosition = playerLoader.item.position;
                            onlinePlayerViewModel.changeVideoQuality(`sd`);
                            localStorage.setVideoQuality(0);
                        }
                        onButtonAlreadyClicked: {
                            userConfigurationViewModel.hidedQuality = !userConfigurationViewModel.hidedQuality;
                        }
                    }
                    Rectangle {
                        width: 20
                        height: 20
                        visible: onlinePlayerViewModel.videoDuration > 0
                        color: "transparent"
                        PlainText {
                            anchors.centerIn: parent
                            text: "|"
                            fontPointSize: 10
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: userConfigurationViewModel.hidedSpeed && !isChecked ? 0 : 40
                        visible: onlinePlayerViewModel.videoDuration > 0 && !(userConfigurationViewModel.hidedSpeed && !isChecked)
                        text: "x0.25"
                        isChecked: onlinePlayerViewModel.playbackRate === 0.25
                        onButtonClicked: {
                            onlinePlayerViewModel.setVideoSpeed(0.25)
                        }
                        onButtonAlreadyClicked: {
                            userConfigurationViewModel.hidedSpeed = !userConfigurationViewModel.hidedSpeed;
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: userConfigurationViewModel.hidedSpeed && !isChecked ? 0 : 40
                        visible: onlinePlayerViewModel.videoDuration > 0 && !(userConfigurationViewModel.hidedSpeed && !isChecked)
                        text: "x0.5"
                        isChecked: onlinePlayerViewModel.playbackRate === 0.5
                        onButtonClicked: {
                            onlinePlayerViewModel.setVideoSpeed(0.5);
                        }
                        onButtonAlreadyClicked: {
                            userConfigurationViewModel.hidedSpeed = !userConfigurationViewModel.hidedSpeed;
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: userConfigurationViewModel.hidedSpeed && !isChecked ? 0 : 40
                        visible: onlinePlayerViewModel.videoDuration > 0 && !(userConfigurationViewModel.hidedSpeed && !isChecked)
                        text: "x0.75"
                        isChecked: onlinePlayerViewModel.playbackRate === 0.75
                        onButtonClicked: {
                            onlinePlayerViewModel.setVideoSpeed(0.75);
                        }
                        onButtonAlreadyClicked: {
                            userConfigurationViewModel.hidedSpeed = !userConfigurationViewModel.hidedSpeed;
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: userConfigurationViewModel.hidedSpeed && !isChecked ? 0 : 40
                        visible: onlinePlayerViewModel.videoDuration > 0 && !(userConfigurationViewModel.hidedSpeed && !isChecked)
                        text: "x1"
                        isChecked: onlinePlayerViewModel.playbackRate === 1
                        onButtonClicked: {
                            onlinePlayerViewModel.setVideoSpeed(1);
                        }
                        onButtonAlreadyClicked: {
                            userConfigurationViewModel.hidedSpeed = !userConfigurationViewModel.hidedSpeed;
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: userConfigurationViewModel.hidedSpeed && !isChecked ? 0 : 40
                        visible: onlinePlayerViewModel.videoDuration > 0 && !(userConfigurationViewModel.hidedSpeed && !isChecked)
                        text: "x1.25"
                        isChecked: onlinePlayerViewModel.playbackRate === 1.1
                        onButtonClicked: {
                            onlinePlayerViewModel.setVideoSpeed(1.1);
                        }
                        onButtonAlreadyClicked: {
                            userConfigurationViewModel.hidedSpeed = !userConfigurationViewModel.hidedSpeed;
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: userConfigurationViewModel.hidedSpeed && !isChecked ? 0 : 40
                        visible: onlinePlayerViewModel.videoDuration > 0 && !(userConfigurationViewModel.hidedSpeed && !isChecked)
                        text: "x1.5"
                        isChecked: onlinePlayerViewModel.playbackRate === 1.2
                        onButtonClicked: {
                            onlinePlayerViewModel.setVideoSpeed(1.2);
                        }
                        onButtonAlreadyClicked: {
                            userConfigurationViewModel.hidedSpeed = !userConfigurationViewModel.hidedSpeed;
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: userConfigurationViewModel.hidedSpeed && !isChecked ? 0 : 40
                        visible: onlinePlayerViewModel.videoDuration > 0 && !(userConfigurationViewModel.hidedSpeed && !isChecked)
                        text: "x1.75"
                        isChecked: onlinePlayerViewModel.playbackRate === 1.3
                        onButtonClicked: {
                            onlinePlayerViewModel.setVideoSpeed(1.3);
                        }
                        onButtonAlreadyClicked: {
                            userConfigurationViewModel.hidedSpeed = !userConfigurationViewModel.hidedSpeed;
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: userConfigurationViewModel.hidedSpeed && !isChecked ? 0 : 40
                        visible: onlinePlayerViewModel.videoDuration > 0 && !(userConfigurationViewModel.hidedSpeed && !isChecked)
                        text: "x2"
                        isChecked: onlinePlayerViewModel.playbackRate === 1.5
                        onButtonClicked: {
                            onlinePlayerViewModel.setVideoSpeed(1.5);
                        }
                        onButtonAlreadyClicked: {
                            userConfigurationViewModel.hidedSpeed = !userConfigurationViewModel.hidedSpeed;
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: userConfigurationViewModel.hidedSpeed && !isChecked ? 0 : 40
                        visible: onlinePlayerViewModel.videoDuration > 0 && !(userConfigurationViewModel.hidedSpeed && !isChecked)
                        text: "x3"
                        isChecked: onlinePlayerViewModel.playbackRate === 2
                        onButtonClicked: {
                            onlinePlayerViewModel.setVideoSpeed(2);
                        }
                        onButtonAlreadyClicked: {
                            userConfigurationViewModel.hidedSpeed = !userConfigurationViewModel.hidedSpeed;
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
                        hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                        iconPath: onlinePlayerViewModel.muted ? applicationThemeViewModel.currentItems.iconPlayerMute : applicationThemeViewModel.currentItems.iconPlayerUnMute
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            playerLoader.item.muted = !playerLoader.item.muted;
                            onlinePlayerViewModel.muted = playerLoader.item.muted;
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
                                localStorage.setVolume(playerLoader.item.volume);
                            }
                            controlPanel.forceActiveFocus();
                        }
                        onMoved: {
                            playerLoader.item.volume = value / 100;
                            if (playerLoader.item.muted) playerLoader.item.muted = false;
                        }
                    }
                    IconButton {
                        id: helpbutton
                        visible: !mainViewModel.isSmallSizeMode
                        width: 40
                        height: 40
                        hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                        iconPath: applicationThemeViewModel.currentItems.iconPlayerInfo
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            helpPlayerPopup.open();
                        }

                        DefaultPopup {
                            id: helpPlayerPopup
                            x: helpbutton.width - 300
                            y: helpbutton.height - 240
                            width: 300
                            height: 240

                            modal: true
                            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                            PlainText {
                                text: "Горячие клавиши:\n" +
                                      "Следующая серия - Page Down, N\n" +
                                      "Предыдущая серия - Page Up, P\n" +
                                      "Полный экран - F11, F\n" +
                                      "Увеличение громкости - Up\n" +
                                      "Уменьшение громкости - Down\n" +
                                      "Mute - M\n" +
                                      "Поверх всех окон (вкл/выкл) - T\n" +
                                      "Прыжок влево - Left\n" +
                                      "Прыжок вправо - Right\n" +
                                      "Поверх всех окон вкл - Home\n" +
                                      "Поверх всех окон выкл - End\n" +
                                      "Воспроизведение/Пауза - Space\n" +
                                      "Вернуться в каталог релизов - Esc"
                                fontPointSize: 10
                                verticalAlignment: Text.AlignVCenter
                                anchors.fill: parent
                            }

                        }
                    }
                }

                Row {
                    anchors.centerIn: parent
                    spacing: 5
                    IconButton {
                        width: 40
                        height: 40
                        visible: onlinePlayerViewModel.videoDuration > 0 && !mainViewModel.isSmallSizeMode
                        hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                        iconPath: applicationThemeViewModel.currentItems.iconPlayerJumpLeft
                        iconWidth: 24
                        iconHeight: 24
                        tooltipMessage: "Прыжок назад"
                        onButtonPressed: {
                            playerLoader.item.seek(onlinePlayerViewModel.jumpInPlayer(jumpMinuteComboBox.currentIndex, jumpSecondComboBox.currentIndex, true))
                        }
                    }
                    IconButton {
                        id: prevButton
                        width: 40
                        height: 40
                        hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                        iconPath: applicationThemeViewModel.currentItems.iconPlayerPreviousSeria
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            onlinePlayerViewModel.previousVideo();
                        }
                        tooltipMessage: "Предыдущая серия (P / PgUp)"
                    }
                    IconButton {
                        id: playButton
                        visible: false
                        width: 40
                        height: 40
                        hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                        iconPath: applicationThemeViewModel.currentItems.iconPlayerPlay
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            playerLoader.item.play();
                        }
                    }
                    IconButton {
                        id: pauseButton
                        visible: false
                        width: 40
                        height: 40
                        hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                        iconPath: applicationThemeViewModel.currentItems.iconPlayerPause
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            playerLoader.item.pause();
                        }
                    }
                    IconButton {
                        id: nextButton
                        width: 40
                        height: 40
                        hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                        iconPath: applicationThemeViewModel.currentItems.iconPlayerNextSeria
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            onlinePlayerViewModel.nextVideo();
                        }
                        tooltipMessage: "Следующая серия (N / PgDn)"
                    }
                    IconButton {
                        width: 40
                        height: 40
                        visible: onlinePlayerViewModel.videoDuration > 0 && !mainViewModel.isSmallSizeMode
                        hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                        iconPath: applicationThemeViewModel.currentItems.iconPlayerJumpRight
                        iconWidth: 24
                        iconHeight: 24
                        tooltipMessage: "Прыжок вперед"
                        onButtonPressed: {
                            playerLoader.item.seek(onlinePlayerViewModel.jumpInPlayer(jumpMinuteComboBox.currentIndex, jumpSecondComboBox.currentIndex, false));
                        }
                    }
                }

                Row {
                    spacing: 5
                    anchors.right: parent.right
                    anchors.rightMargin: 6
                    anchors.verticalCenter: parent.verticalCenter

                    IconButton {
                        id: reloadButton
                        width: 40
                        height: 40
                        visible: !autoTopMost.checked
                        hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                        iconPath: applicationThemeViewModel.currentItems.iconPlayerRefresh
                        iconWidth: 29
                        iconHeight: 29
                        tooltipMessage: "Перезапустить текущее видео"
                        onButtonPressed: {
                            onlinePlayerViewModel.reloadCurrentVideo()
                        }
                    }

                    IconButton {
                        id: topmostButton
                        width: 40
                        height: 40
                        visible: !autoTopMost.checked && !mainViewModel.isSmallSizeMode
                        hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                        iconPath: applicationThemeViewModel.currentItems.iconPlayerTopMost
                        iconWidth: 29
                        iconHeight: 29
                        tooltipMessage: windowSettings.isTopMost ? "Выключить режим поверх всех окон (T)" : "Включить режим поверх всех окон (T)"
                        onButtonPressed: {
                            windowSettings.toggleStayOnTopMode();
                        }
                    }

                    IconButton {
                        id: remotePlayerButton
                        visible: !mainViewModel.isSmallSizeMode
                        width: 40
                        height: 40
                        hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                        iconPath: applicationThemeViewModel.currentItems.iconPlayerRemote
                        iconWidth: 24
                        iconHeight: 24
                        tooltipMessage: onlinePlayerViewModel.remotePlayer.started ? "Удаленный плеер включен" : "Удаленный плеер выключен, откройте настройки для подключения"
                        onButtonPressed: {
                            remotePlayerPopup.open();
                        }

                        DefaultPopup {
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

                                CommonSwitch {
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

                                CommonSwitch {
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

                                CommonSwitch {
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
                        hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                        iconPath: applicationThemeViewModel.currentItems.iconPlayerSettings
                        iconWidth: 24
                        iconHeight: 24
                        tooltipMessage: "Показать настройки страницы Видеоплеер"
                        onButtonPressed: {
                            optionsPopup.open();
                        }

                        DefaultPopup {
                            id: optionsPopup
                            x: optionsButton.width - 610
                            y: optionsButton.height - 510
                            width: 600
                            height: 500

                            modal: true
                            focus: true
                            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                            Column {
                                id: leftColumn
                                width: 300
                                spacing: 10
                                PlainText {
                                    width: leftColumn.width - 20
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
                                    width: leftColumn.width - 20
                                    fontPointSize: 10
                                    text: "Автопереход между сериями"
                                }

                                CommonSwitch {
                                    id: autoNextVideo
                                    onCheckedChanged: {
                                        localStorage.setAutoNextVideo(checked);
                                    }
                                }

                                PlainText {
                                    width: leftColumn.width - 20
                                    fontPointSize: 10
                                    text: "Автопереход в режим поверх всех окон"
                                }

                                CommonSwitch {
                                    id: autoTopMost
                                    onCheckedChanged: {
                                        localStorage.setAutoTopMost(checked);
                                        if (!checked) windowSettings.unsetStayOnTop()
                                    }
                                }

                                PlainText {
                                    width: leftColumn.width - 20
                                    fontPointSize: 10
                                    text: "Показывать постер релиза"
                                }

                                CommonSwitch {
                                    id: showReleaseInfo
                                    onCheckedChanged: {
                                        localStorage.setShowReleaseInfo(checked);
                                        if (!checked) releasePosterArea.visible = false;
                                    }
                                }

                                PlainText {
                                    width: optionsPopup.width - 20
                                    fontPointSize: 10
                                    text: "Показывать видео превью"
                                }

                                CommonSwitch {
                                    id: showVideoPreview
                                    onCheckedChanged: {
                                        localStorage.setShowVideoPreview(checked);
                                    }
                                }

                                PlainText {
                                    width: leftColumn.width
                                    fontPointSize: 10
                                    text: "Прозрачность панели"
                                }
                                Slider {
                                    width: leftColumn.width - 20
                                    height: 40
                                    id: opacitySlider
                                    from: 0
                                    to: 110
                                    onPressedChanged: {
                                        controlPanel.forceActiveFocus();
                                    }
                                    onMoved: {
                                        controlPanelBackground.opacity = value / 100;
                                        console.log(controlPanelBackground.opacity);
                                        userConfigurationViewModel.opacityPlayerPanel = value;
                                    }
                                    Component.onCompleted: {
                                        value = userConfigurationViewModel.opacityPlayerPanel;
                                        controlPanelBackground.opacity = value / 100;
                                    }
                                }
                            }

                            Column {
                                id: rightColumn
                                anchors.left: leftColumn.right
                                width: 300
                                spacing: 10

                                PlainText {
                                    width: rightColumn.width - 20
                                    fontPointSize: 10
                                    text: "Автопропуск опенинга"
                                }
                                CommonSwitch {
                                    id: autoSkipOpeningSwitch
                                    checked: userConfigurationViewModel.autoSkipOpening
                                    onCheckedChanged: {
                                        userConfigurationViewModel.autoSkipOpening = checked;
                                    }
                                }
                                PlainText {
                                    width: rightColumn.width - 20
                                    fontPointSize: 10
                                    text: "Выравнивание списка серий по центру"
                                }
                                CommonSwitch {
                                    id: fixedControlPanelSwitch
                                    checked: userConfigurationViewModel.fixedControlPanel
                                    onCheckedChanged: {
                                        userConfigurationViewModel.fixedControlPanel = checked;
                                    }
                                }
                                PlainText {
                                    width: rightColumn.width - 20
                                    fontPointSize: 10
                                    text: "Скрывать кнопку Пропуcтить опенинг"
                                }
                                CommonSwitch {
                                    id: hideSkipOpeningSwitch
                                    checked: userConfigurationViewModel.hideSkipOpening
                                    onCheckedChanged: {
                                        userConfigurationViewModel.hideSkipOpening = checked;
                                    }
                                }

                                PlainText {
                                    width: rightColumn.width - 20
                                    fontPointSize: 10
                                    text: "Текущий плеер"
                                }

                                CommonComboBox {
                                    id: playersComboBox
                                    model: onlinePlayerWindowViewModel.players

                                    onActivated: {
                                        optionsPopup.close();
                                        onlinePlayerViewModel.restorePosition = onlinePlayerViewModel.videoPosition;
                                        const newPlayer = onlinePlayerWindowViewModel.players[currentIndex];
                                        onlinePlayerWindowViewModel.changePlayer(newPlayer);
                                        if (userConfigurationViewModel.needSavePlayer) userConfigurationViewModel.lastSelectedPlayer = newPlayer;
                                    }

                                    Connections {
                                        target: onlinePlayerWindowViewModel
                                        function onSelectedPlayerChanged() {
                                            const playerIndex = onlinePlayerWindowViewModel.players.indexOf(onlinePlayerWindowViewModel.selectedPlayer);
                                            if (playersComboBox.currentIndex !== playerIndex) playersComboBox.currentIndex = playerIndex;
                                        }
                                    }

                                    Component.onCompleted: {
                                        const playerIndex = onlinePlayerWindowViewModel.players.indexOf(onlinePlayerWindowViewModel.selectedPlayer);
                                        if (playersComboBox.currentIndex !== playerIndex) playersComboBox.currentIndex = playerIndex;
                                    }
                                }

                                PlainText {
                                    width: rightColumn.width - 20
                                    fontPointSize: 10
                                    text: "Сохранять выбранный плеер"
                                }
                                CommonSwitch {
                                    id: keepSelectedPlayerSwitch
                                    checked: userConfigurationViewModel.needSavePlayer
                                    onCheckedChanged: {
                                        userConfigurationViewModel.needSavePlayer = checked;
                                        if (!checked) {
                                            userConfigurationViewModel.lastSelectedPlayer = "";
                                        }
                                    }
                                }
                            }
                        }
                    }
                    IconButton {
                        id: resizeVideoButton
                        visible: !mainViewModel.isSmallSizeMode
                        width: 40
                        height: 40
                        hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                        iconPath: applicationThemeViewModel.currentItems.iconPlayerCropMode
                        iconWidth: 29
                        iconHeight: 29
                        tooltipMessage: "Включить режим обрезки видео потока"
                        onButtonPressed: {
                            switch (playerLoader.item.fillMode) {
                                case VideoOutput.PreserveAspectFit:
                                    playerLoader.item.fillMode = VideoOutput.PreserveAspectCrop;
                                    userConfigurationViewModel.isCroppedPlayer = true;
                                    break;
                                case VideoOutput.PreserveAspectCrop:
                                    playerLoader.item.fillMode = VideoOutput.PreserveAspectFit;
                                    userConfigurationViewModel.isCroppedPlayer = false;
                                    break;
                            }
                        }
                    }
                    IconButton {
                        id: fullScreenButton
                        width: 40
                        height: 40
                        hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                        iconPath: applicationThemeViewModel.currentItems.iconPlayerFullscreen
                        iconWidth: 29
                        iconHeight: 29
                        tooltipMessage: "Переключиться между полноэкранным и оконным режимами"
                        onButtonPressed: {
                            onlinePlayerViewModel.toggleFullScreen();
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
        id: mask
        width: 180
        height: 260
        radius: 10
        visible: false
    }

    Rectangle {
        id: skipOpening
        visible: onlinePlayerViewModel.displaySkipOpening && !userConfigurationViewModel.hideSkipOpening
        anchors.bottom: controlPanel.top
        anchors.right: controlPanel.right
        anchors.rightMargin: onlinePlayerViewModel.showNextPosterRelease ? 230 : 10
        anchors.bottomMargin: 4
        width: 200
        height: 40
        color: applicationThemeViewModel.playerControlBackground
        border.width: 1
        border.color: applicationThemeViewModel.plainTextColor

        PlainText {
            anchors.centerIn: parent
            fontPointSize: 12
            text: "Пропустить опенинг"
        }

        MouseArea {
            enabled: skipOpening.visible
            anchors.fill: parent
            onPressed: {
                const position = onlinePlayerViewModel.skipOpening();
                if (onlinePlayerViewModel.restorePosition != 0) onlinePlayerViewModel.restorePosition = 0;
                playerLoader.item.seek(position);
                console.log('playerLoader.item.seek(position);');
            }
        }
    }

    Connections {
        target: onlinePlayerViewModel
        function onDisplaySkipOpeningChanged() {
            if (userConfigurationViewModel.autoSkipOpening) {
                if (onlinePlayerViewModel.displaySkipOpening && !onlinePlayerViewModel.endSkipOpening) {
                    const position = onlinePlayerViewModel.skipOpening();
                    playerLoader.item.seek(position);
                    console.log('playerLoader.item.seek(position); автопропуск');
                    notificationViewModel.sendInfoNotification(`Произошел автоматический пропуск опенинга`);
                }
            }
        }
    }

    Item {
        id: releasePosterArea
        anchors.right: parent.right
        anchors.top: parent.top
        width: 182
        height: 272

        CorneredImage {
            anchors.centerIn: parent
            width: 180
            height: 270
            posterSource: localStorage.getReleasePosterPath(onlinePlayerViewModel.selectedRelease, onlinePlayerViewModel.releasePoster)
        }
        MouseArea {
            anchors.fill: parent
            onPressed: {
                mainViewModel.selectPage("release:" + onlinePlayerViewModel.selectedRelease);
            }
        }
    }

    InfoAboutPlayer {
        anchors.right: releasePosterArea.left
        anchors.top: parent.top
    }

    Rectangle {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 100
        color: applicationThemeViewModel.playerControlBackground
        visible: onlinePlayerViewModel.showNextPosterRelease
        width: 220
        height: 300
        radius: 12
        MouseArea {
            anchors.fill: parent
            onPressed: {
                const nextReleaseId = releaseLinkedSeries.getNextLinkedRelease(onlinePlayerViewModel.selectedRelease);
                if (nextReleaseId > 0) {
                    onlinePlayerViewModel.customPlaylistPosition = -1;
                    onlinePlayerViewModel.navigateReleaseId = nextReleaseId;
                    onlinePlayerViewModel.navigateVideos = releasesViewModel.getReleaseVideos(nextReleaseId);
                    onlinePlayerViewModel.navigatePoster = releasesViewModel.getReleasePoster(nextReleaseId);

                    onlinePlayerViewModel.setupForSingleRelease();
                }
            }
        }

        PlainText {
            anchors.top: parent.top
            anchors.topMargin: 2
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Нажмите чтобы перейти к"
            fontPointSize: 10
            wrapMode: Text.WordWrap
        }

        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 4
            width: 182
            height: 272
            border.color: applicationThemeViewModel.posterBorder
            border.width: 1
            Image {
                anchors.centerIn: parent
                source: onlinePlayerViewModel.nextReleasePoster
                fillMode: Image.PreserveAspectCrop
                width: 180
                height: 270
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

    function setSerieScrollPosition() {
        let newPosition = onlinePlayerViewModel.selectedVideo * 40 - serieScrollContainer.height;
        newPosition += 40;
        if (newPosition < 0) newPosition = 0;
        serieScrollContainer.contentY = newPosition;
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
        volumeSlider.value = playerLoader.item.volume * 100;
    }
}
