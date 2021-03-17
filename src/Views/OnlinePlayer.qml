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
    property bool isFullScreen: false
    property int selectedRelease: -1
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
    property var ports: [12345, 34560, 52354, 67289]
    property real lastMouseYPosition: 0
    property string releasePoster: ""
    property bool isCinemahall: false
    property var cinemahallReleases: []
    property string videoSourceChangedCommand: `videosourcechanged`
    property string videoPositionChangedCommand: `positionchanged`
    property string videoVolumeChangedCommand: `volumechanged`
    property string videoPlaybackRateCommand: `playbackratechanged`
    property string videoPlaybackCommand: `playbackchanged`

    signal navigateFrom()
    signal setReleaseVideo()
    signal setCinemahallVideo()
    signal changeFullScreenMode(bool fullScreen)
    signal navigateTo()
    signal returnToReleasesPage()
    signal windowNotActived()
    signal receiveRemoteCommand(int id, string command, string argument)

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
        if (event.key === Qt.Key_Home && !autoTopMost.checked) windowSettings.setStayOnTop();
        if (event.key === Qt.Key_End && !autoTopMost.checked) windowSettings.unsetStayOnTop();
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
        showReleaseInfo.checked = userSettings.showReleaseInfo;
        sendVolumeToRemoteSwitch.checked = applicationSettings.sendVolumeToRemote;
        sendPlaybackToRemoteSwitch.checked = applicationSettings.sendPlaybackToRemote;
        remotePlayer.port = applicationSettings.remotePort;
        remotePlayerPortComboBox.currentIndex = ports.indexOf(applicationSettings.remotePort);

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

        if (!_page.setReleaseParameters.releaseId && !_page.isCinemahall) {
            const lastSeenObject = localStorage.getLastVideoSeen();
            if (lastSeenObject === ``) return;

            const seenObject = JSON.parse(lastSeenObject);
            const release = JSON.parse(localStorage.getRelease(seenObject.id));
            _page.releasePoster = release.poster;
            _page.setReleaseParameters = {
                releaseId: seenObject.id,
                videos: release.videos,
                customPlaylistPosition: -1
            };

            _page.setReleaseVideo();
        }
    }

    onSetReleaseVideo: {
        _page.isCinemahall = false;
        const jsonVideos = JSON.parse(_page.setReleaseParameters.videos);
        const seenJson = localStorage.getVideoSeen(_page.setReleaseParameters.releaseId);
        _page.seenVideo = {};
        if (seenJson) _page.seenVideo = JSON.parse(seenJson);
        _page.selectedRelease = _page.setReleaseParameters.releaseId;

        const release = JSON.parse(localStorage.getRelease(_page.setReleaseParameters.releaseId));
        _page.releasePoster = release.poster;

        const releaseVideos = [];

        for (let i = 0; i < jsonVideos.length; i++) {
            const video = jsonVideos[i];
            releaseVideos.push(
                {
                    title: video.title,
                    sd: video.sd,
                    id: video.id,
                    hd: video.hd,
                    fullhd: video.fullhd,
                    sdfile: video.srcSd,
                    hdfile: video.srcHd,
                    releaseId: _page.setReleaseParameters.releaseId,
                    releasePoster: release.poster,
                    isGroup: false
                }
            );
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

        setVideoSource(firstVideo[_page.videoQuality]);
        player.play();

        localStorage.setToReleaseHistory(_page.setReleaseParameters.releaseId, 1);

        setSerieScrollPosition();
    }

    onSetCinemahallVideo: {
        _page.isCinemahall = true;
        _page.cinemahallReleases = JSON.parse(localStorage.getCinemahallReleases());

        const releaseVideos = [];

        for (let i = 0; i < _page.cinemahallReleases.length; i++) {
            const release = _page.cinemahallReleases[i];
            const videos = JSON.parse(release.videos);
            releaseVideos.push({ title: release.title, isGroup: true });

            videos.sort(
                (left, right) => {
                    if (left.id === right.id) return 0;
                    return left.id > right.id ? 1 : -1;
                }
            );

            for (var l = 0; l < videos.length; l++) {
                const video = videos[l];
                releaseVideos.push(
                    {
                        title: video.title,
                        sd: video.sd,
                        id: video.id,
                        hd: video.hd,
                        fullhd: video.fullhd,
                        sdfile: video.srcSd,
                        hdfile: video.srcHd,
                        releaseId: release.id,
                        releasePoster: release.poster,
                        order: l,
                        isGroup: false
                    }
                );
            }
        }

        _page.releaseVideos = releaseVideos;

        refreshSeenMarks();

        let firstVideo = releaseVideos.filter(a => !a.isGroup).find(
            (releaseVideo) => !(releaseVideo.releaseId in _page.seenMarks && releaseVideo.order in _page.seenMarks[releaseVideo.releaseId])
        );

        if (firstVideo) {
            _page.selectedRelease = firstVideo.releaseId;
            setReleasePoster(firstVideo.releasePoster, firstVideo.releaseId);

            _page.selectedVideo = firstVideo.order;
            _page.isFullHdAllowed = "fullhd" in firstVideo;
            if (!firstVideo[_page.videoQuality]) _page.videoQuality = "sd";

            setVideoSource(firstVideo[_page.videoQuality]);
            player.play();

            localStorage.setToReleaseHistory(firstVideo.releaseId, 1);

            setSerieScrollPosition();
            _page.seenVideo = JSON.parse(localStorage.getVideoSeen(firstVideo.releaseId));
            if (_page.seenVideo.id && _page.seenVideo.videoId !== firstVideo.order) _page.seenVideo.videoPosition = 0;
        } else {
            setVideoSource("");
        }
    }

    onReceiveRemoteCommand: {
        switch (command){
            case `getcurrentvideosource`:
                remotePlayer.sendCommandToUser(id, _page.videoSourceChangedCommand, _page.videoSource);
                break;
            case `getcurrentvideoposition`:
                remotePlayer.sendCommandToUser(id, _page.videoPositionChangedCommand, player.position.toString() + `/` + player.duration.toString());
                break;
            case `getcurrentvolume`:
                if (sendVolumeToRemoteSwitch.checked) remotePlayer.sendCommandToUser(id, _page.videoVolumeChangedCommand, volumeSlider.value.toString());
                break;
            case `getcurrentplaybackrate`:
                remotePlayer.sendCommandToUser(id, _page.videoPlaybackRateCommand, _page.videoSpeed.toString());
                break;
            case `getcurrentplayback`:
                if (player.playbackState === MediaPlayer.PausedState && sendPlaybackToRemoteSwitch.checked) remotePlayer.sendCommandToUser(id, _page.videoPlaybackCommand, "pause");
                break;
        }
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
        onPlaybackStateChanged: {
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

            if (playbackState === MediaPlayer.PlayingState) remotePlayer.broadcastCommand(_page.videoPlaybackCommand, "play");
            if (playbackState === MediaPlayer.PausedState) remotePlayer.broadcastCommand(_page.videoPlaybackCommand, "pause");
        }
        onVolumeChanged: {
            volumeSlider.value = volume * 100;
            if (applicationSettings.sendVolumeToRemote) remotePlayer.broadcastCommand(_page.videoVolumeChangedCommand, volumeSlider.value.toString());
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
            if (!playerLocation.pressed && _page.lastMovedPosition === 0) playerLocation.value = position;

            _page.displayVideoPosition = `${_page.getDisplayTimeFromSeconds(position / 1000)} из ${_page.getDisplayTimeFromSeconds(duration / 1000)}`;
            _page.displayEndVideoPosition = _page.getDisplayTimeFromSeconds((duration - position) / 1000);

            if (_page.positionIterator < 20) _page.positionIterator++;

            if (_page.positionIterator >= 20) {
                _page.positionIterator = 0;
                localStorage.setVideoSeens(_page.selectedRelease, _page.selectedVideo, position);
            }

            if (!(_page.selectedRelease in _page.seenMarks && _page.selectedVideo in _page.seenMarks[_page.selectedRelease])) {
                if (duration > 0 && position > 0) {
                    const positionPercent = position / duration * 100;
                    if (positionPercent >= 90) {
                        let seenMarks = getReleaseSeens(_page.selectedRelease);
                        seenMarks[_page.selectedVideo] = true;
                        const obj = _page.seenMarks;
                        _page.seenMarks = obj;
                        localStorage.setSeenMark(_page.selectedRelease, _page.selectedVideo, true);
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
                            height:  modelData.isGroup ? 70 : 40
                            width: seriesPopup.width
                            color: _page.selectedVideo === modelData.order && _page.selectedRelease === modelData.releaseId ? ApplicationTheme.playlistSelectedBackground : ApplicationTheme.playlistBackground
                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                onEntered: {
                                    if (playerTimer.running) playerTimer.stop();
                                }
                                onClicked: {
                                    if (modelData.isGroup) return;

                                    _page.selectedVideo = modelData.order;
                                    _page.isFullHdAllowed = "fullhd" in modelData;
                                    setVideoSource(modelData[_page.videoQuality]);
                                    _page.selectedRelease = modelData.releaseId;
                                    setReleasePoster(modelData.releasePoster, modelData.releaseId);
                                    player.play();
                                }
                            }
                            Text {
                                visible: !modelData.isGroup
                                color: _page.selectedVideo === modelData.order && _page.selectedRelease === modelData.releaseId ? ApplicationTheme.playlistSelectedText : ApplicationTheme.playlistText
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                text: modelData.title
                            }

                            Rectangle {
                                color: "transparent"
                                anchors.fill: parent
                                anchors.leftMargin: 10
                                anchors.rightMargin: 10

                                Text {
                                    visible: modelData.isGroup
                                    color: _page.selectedVideo === modelData.order && _page.selectedRelease === modelData.releaseId ? ApplicationTheme.playlistSelectedText : ApplicationTheme.playlistText
                                    text: modelData.title
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
                                    visible: !modelData.isGroup
                                    iconColor: ApplicationTheme.filterIconButtonColor
                                    hoverColor: ApplicationTheme.filterIconButtonHoverColor
                                    iconPath: _page.seenMarks && modelData.releaseId in _page.seenMarks && modelData.order in _page.seenMarks[modelData.releaseId] ? "../Assets/Icons/seenmarkselected.svg" : "../Assets/Icons/seenmark.svg"
                                    iconWidth: 22
                                    iconHeight: 22
                                    onButtonHoverEnter: {
                                        if (playerTimer.running) playerTimer.stop();
                                    }
                                    onButtonPressed: {
                                        let newState = false;
                                        let seenMarks = getReleaseSeens(modelData.releaseId);

                                        if (seenMarks[modelData.order]) {
                                            delete seenMarks[modelData.order];
                                            newState = false;
                                        } else {
                                            seenMarks[modelData.order] = true;
                                            newState = true;

                                        }

                                        const oldSeenMarks = _page.seenMarks;
                                        _page.seenMarks = {};
                                        _page.seenMarks = oldSeenMarks;

                                        localStorage.setSeenMark(modelData.releaseId, modelData.order, newState);
                                    }
                                }

                                IconButton {
                                    height: 36
                                    width: 36
                                    visible: !modelData.isGroup
                                    iconColor: ApplicationTheme.filterIconButtonColor
                                    hoverColor: ApplicationTheme.filterIconButtonHoverColor
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
                    if (!pressed && _page.lastMovedPosition > 0) {
                        player.seek(_page.lastMovedPosition);
                        remotePlayer.broadcastCommand(_page.videoPositionChangedCommand, _page.lastMovedPosition.toString() + `/` + player.duration.toString());
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

                PlainText {
                    text: _page.displayVideoPosition
                    fontPointSize: 10
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
                            setVideoSource(video[_page.videoQuality]);
                            player.start();

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
                            setVideoSource(video[_page.videoQuality]);
                            player.start();

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
                            setVideoSource(video[_page.videoQuality]);
                            player.start();

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
                        isChecked: _page.videoSpeed === 0.25
                        onButtonClicked: {
                            setVideoSpeed(0.25)
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x0.5"
                        isChecked: _page.videoSpeed === 0.5
                        onButtonClicked: {
                            setVideoSpeed(0.5);
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x0.75"
                        isChecked: _page.videoSpeed === 0.75
                        onButtonClicked: {
                            setVideoSpeed(0.75);
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x1"
                        isChecked: _page.videoSpeed === 1
                        onButtonClicked: {
                            setVideoSpeed(1);
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x1.25"
                        isChecked: _page.videoSpeed === 1.1
                        onButtonClicked: {
                            setVideoSpeed(1.1);
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x1.5"
                        isChecked: _page.videoSpeed === 1.2
                        onButtonClicked: {
                            setVideoSpeed(1.2);
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x1.75"
                        isChecked: _page.videoSpeed === 1.3
                        onButtonClicked: {
                            setVideoSpeed(1.3);
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x2"
                        isChecked: _page.videoSpeed === 1.5
                        onButtonClicked: {
                            setVideoSpeed(1.5);
                        }
                    }
                    ToggleButton {
                        height: 20
                        width: 40
                        visible: player.duration > 0
                        text: "x3"
                        isChecked: _page.videoSpeed === 2
                        onButtonClicked: {
                            setVideoSpeed(2);
                        }
                    }
                }

                PlainText {
                    height: 20
                    anchors.right: parent.right
                    anchors.rightMargin: 4
                    text: _page.displayEndVideoPosition
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
                            _page.previousVideo();
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
                            _page.nextVideo();
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
                        iconColor: remotePlayer.started ? ApplicationTheme.filterIconButtonGreenColor : ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
                        iconPath: "../Assets/Icons/connect.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            remotePlayerPopup.open();
                        }

                        ToolTip.delay: 1000
                        ToolTip.visible: remotePlayerButton.hovered
                        ToolTip.text: remotePlayer.started ? "Удаленный плеер включен" : "Удаленный плеер выключен, откройте настройки для подключения"

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
                                            remotePlayer.port = applicationSettings.remotePort;
                                            remotePlayer.startServer();
                                        } else {
                                            remotePlayer.stopServer();
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
                                    }
                                }

                                PlainText {
                                    width: optionsPopup.width - 20
                                    fontPointSize: 10
                                    text: "Подключено: " + remotePlayer.countUsers
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
                                            localStorage.setJumpMinute(_page.jumpMinutes[index]);
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
                                            localStorage.setJumpSecond(_page.jumpSeconds[index]);
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
                            toggleFullScreen();
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
            source: _page.releasePoster
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
        visible: _page.isBuffering
        anchors.centerIn: parent
        AnimatedImage {
            id: spinner
            anchors.centerIn: parent
            paused: !_page.visible
            playing: _page.visible
            source: _page.visible ? "../Assets/Icons/spinner.gif" : ""
        }
    }

    function setReleasePoster(poster, releaseId) {
        let posterPath = poster ? localStorage.getReleasePosterPath(releaseId, poster) : '../Assets/Icons/donate.jpg';
        _page.releasePoster = posterPath;
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

    function checkExistingVideoQuality(video) {
        if (video[_page.videoQuality]) {
            return video[_page.videoQuality];
        } else {
            if (`sd` in video) {
                _page.videoQuality = `sd`;
                return video['sd'];
            }
            if (`hd` in video) {
                _page.videoQuality = `hd`;
                return video['hd'];
            }
            if (`fullhd` in video) {
                _page.videoQuality = `fullhd`;
                return video['fullhd'];
            }

            return null;
        }        
    }

    function previousNotSeenVideo() {
        let lastNotSeenVideo = null;

        for (const releaseVideo of _page.releaseVideos) {
            if (releaseVideo.isGroup) continue;
            if (releaseVideo.releaseId === _page.selectedRelease && releaseVideo.order === _page.selectedVideo) break;

            if (!(releaseVideo.releaseId in _page.seenMarks && releaseVideo.order in _page.seenMarks[releaseVideo.releaseId])) lastNotSeenVideo = releaseVideo;
        }

        return lastNotSeenVideo;
    }

    function previousVideo() {
        if (_page.selectedVideo === 0) return;
        _page.restorePosition = 0;

        let video;

        if (_page.isCinemahall) {
            const previousVideo = previousNotSeenVideo();
            if (previousVideo) {
                _page.selectedVideo = previousVideo.order;
                video = previousVideo;
            } else {
                return;
            }

        } else {
            _page.selectedVideo--;
            video = _page.releaseVideos[_page.selectedVideo];
        }

        _page.isFullHdAllowed = "fullhd" in video;
        _page.releasePoster = video.releasePoster;
        _page.selectedRelease = video.releaseId;

        setVideoSource(checkExistingVideoQuality(video));

        if (!_page.isCinemahall) setSerieScrollPosition();
    }

    function nextNotSeenVideo() {
        var beforeCurrent = true;
        for (const releaseVideo of _page.releaseVideos) {
            if (releaseVideo.releaseId === _page.selectedRelease && releaseVideo.order <= _page.selectedVideo) {
                beforeCurrent = false;
                continue;
            }
            if (beforeCurrent) continue;
            if (releaseVideo.isGroup) continue;

            if (!(releaseVideo.releaseId in _page.seenMarks && releaseVideo.order in _page.seenMarks[releaseVideo.releaseId])) {
                return releaseVideo;
            }
        }

        return null;
    }

    function setVideoSource(source) {
        _page.videoSource = source;
        remotePlayer.broadcastCommand(_page.videoSourceChangedCommand, source);
    }

    function setVideoSpeed(videoSpeed) {
        _page.videoSpeed = videoSpeed;
        remotePlayer.broadcastCommand(_page.videoPlaybackRateCommand, videoSpeed);
    }

    function nextVideo() {
        _page.restorePosition = 0;
        let video;

        if (_page.isCinemahall) {
            const nextVideo = nextNotSeenVideo();
            if (nextVideo) {
                _page.selectedVideo = nextVideo.order;
                video = nextVideo;
            } else {
                return;
            }
        } else {
            if (_page.selectedVideo === _page.releaseVideos.length - 1) return;

            _page.selectedVideo++;
            video = _page.releaseVideos[_page.selectedVideo];
        }

        _page.isFullHdAllowed = "fullhd" in video;
        _page.releasePoster = video.releasePoster;
        _page.selectedRelease = video.releaseId;

        setVideoSource(checkExistingVideoQuality(video));

        if (!_page.isCinemahall) setSerieScrollPosition();
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
        let releaseIds = [];

        if (_page.isCinemahall) {
            releaseIds = _page.cinemahallReleases.map(a => a.id);
        } else {
            releaseIds.push(_page.setReleaseParameters.releaseId);
        }

        _page.seenMarks = JSON.parse(localStorage.getReleasesSeenMarks(releaseIds));
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

