/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2021 Roman Vladimirov

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
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtMultimedia 5.12
import "../Controls"
import "../Theme"
import "Videoplayer"

ApplicationWindow {
    id: root
    title: '  '
    width: 350
    height: 200
    flags: Qt.Dialog
    minimumWidth: 350
    minimumHeight: 200
    maximumWidth: 500
    maximumHeight: 350

    property var videoSource
    property var videoOutput

    signal showWindow()
    signal hideWindow(bool paused)
    signal closeWindow()

    Loader {
        id: videoOutputLoader
        anchors.fill: parent
        source: onlinePlayerWindowViewModel.isStandartPlayer ? (onlinePlayerWindowViewModel.isQt515 ? `Videoplayer/QtVideo515Output.qml` : `Videoplayer/QtVideoOutput.qml`) : `Videoplayer/QtAvVideoOutput.qml`
        onLoaded: {
            if (!onlinePlayerWindowViewModel.isStandartPlayer) videoOutputLoader.item.source = root.videoSource;
            if (onlinePlayerWindowViewModel.isQt515 && onlinePlayerWindowViewModel.isStandartPlayer) {
                root.videoSource.addNewVideoOuput(videoOutputLoader.item);
            }
            root.videoSource.playbackStateChanged.connect(playbackStateChanged);
            root.videoSource.volumeChanged.connect(volumeChanged);
        }
    }

    MouseArea {
        id: windowedPlayerMouseArea
        width: parent.width
        height: parent.height
        hoverEnabled: true
        cursorShape: onlinePlayerWindowViewModel.windowCursorShape
        onDoubleClicked: {
            window.showPage("videoplayer");
            hideWindow(false);
        }
        onPositionChanged: {
            if (!(root.videoSource.playbackState === MediaPlayer.PlayingState)) {
                if (controlPanel.opacity === 0) onlinePlayerWindowViewModel.showPanel();
                return;
            }

            if (controlPanel.opacity !== 1) onlinePlayerWindowViewModel.showPanel();
            const x = mouse.x;
            const y = mouse.y;

            if (y > root.height - controlPanel.height) {
                windowPlayerTimer.stop();
            } else {
                windowPlayerTimer.restart();
            }
        }
        onExited: {
            if (!windowPlayerTimer.running) windowPlayerTimer.restart();
        }
    }

    Rectangle {
        id: controlPanel
        color: ApplicationTheme.playerControlBackground
        anchors.bottom: parent.bottom
        opacity: onlinePlayerWindowViewModel.panelOpacity
        width: root.width
        height: 60

        RowLayout {
            width: parent.width
            height: parent.height

            Rectangle {
                id: leftPart
                color: "transparent"
                Layout.preferredWidth: 60
                Layout.fillHeight: true

                Slider {
                    id: volumeSlider
                    anchors.centerIn: parent
                    width: parent.width
                    height: 40
                    from: 0
                    value: 10
                    to: 100
                    onPressedChanged: {
                        controlPanel.forceActiveFocus();
                    }
                    onMoved: {
                        root.videoSource.volume = value / 100;
                    }
                }
            }

            Item {
                id: centerPart
                Layout.fillWidth: true
                Layout.fillHeight: true

                Row {
                    anchors.centerIn: parent

                    IconButton {
                        id: previousButton
                        width: 40
                        height: 40
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
                        iconPath: assetsLocation.path + "Icons/step-backward.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            onlinePlayerViewModel.previousVideo();
                        }
                        tooltipMessage: "Предыдущая серия"
                    }

                    IconButton {
                        id: playButton
                        visible: onlinePlayerWindowViewModel.playerButtonVisible
                        width: 40
                        height: 40
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
                        iconPath: assetsLocation.path + "Icons/play-button.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            root.videoSource.play();
                        }
                    }
                    IconButton {
                        id: pauseButton
                        visible: onlinePlayerWindowViewModel.pauseButtonVisible
                        width: 40
                        height: 40
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
                        iconPath: assetsLocation.path + "Icons/pause.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            root.videoSource.pause();
                        }
                    }

                    IconButton {
                        id: nextButton
                        width: 40
                        height: 40
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
                        iconPath: assetsLocation.path + "Icons/step-forward.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            onlinePlayerViewModel.nextVideo();
                        }
                        tooltipMessage: "Следующая серия"
                    }
                }
            }

            Rectangle {
                id: rightPart
                color: "transparent"
                Layout.preferredWidth: 45
                Layout.fillHeight: true

                IconButton {
                    id: topmostButton
                    anchors.centerIn: parent
                    width: 40
                    height: 40
                    iconColor: onlinePlayerWindowViewModel.isTopMost ? ApplicationTheme.filterIconButtonGreenColor : ApplicationTheme.filterIconButtonColor
                    hoverColor: ApplicationTheme.filterIconButtonHoverColor
                    iconPath: assetsLocation.path + "Icons/topmostwindow.svg"
                    iconWidth: 29
                    iconHeight: 29
                    tooltipMessage: onlinePlayerWindowViewModel.isTopMost ? "Выключить режим поверх всех окон" : "Включить режим поверх всех окон"
                    onButtonPressed: {
                        onlinePlayerWindowViewModel.isTopMost = !onlinePlayerWindowViewModel.isTopMost;
                        root.flags = onlinePlayerWindowViewModel.isTopMost ? Qt.Dialog | Qt.WindowStaysOnTopHint : Qt.Dialog;
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
        visible: onlinePlayerViewModel.isBuffering
        anchors.centerIn: parent
        AnimatedImage {
            id: spinner
            anchors.centerIn: parent
            paused: !onlinePlayerWindowViewModel.opened
            playing: onlinePlayerWindowViewModel.opened
            source: assetsLocation.path + "Icons/spinner.gif"
        }
    }

    Timer {
        id: windowPlayerTimer
        interval: 5000
        running: false
        repeat: true
        onTriggered: {
            onlinePlayerWindowViewModel.hideControlPanel()
        }
    }

    function playbackStateChanged() {
        onlinePlayerWindowViewModel.playbackStateChanged(root.videoSource.playbackState === MediaPlayer.PlayingState);
    }

    function volumeChanged() {
        volumeSlider.value = root.videoSource.volume * 100;
        onlinePlayerViewModel.volumeSlider = volumeSlider.value;
    }

    onShowWindow:  {
        videoOutputLoader.item.visible = true;
        show();
        onlinePlayerWindowViewModel.opened = true;
        onlinePlayerWindowViewModel.showPanel();
    }

    onHideWindow: {
        videoOutputLoader.item.visible = false;
        hide();
        onlinePlayerWindowViewModel.opened = false;
        if (paused) root.videoSource.pause();
    }

    onClosing: {
        hideWindow(true);
    }

    onCloseWindow: {
        root.videoSource.playbackStateChanged.disconnect(playbackStateChanged);
        root.videoSource.volumeChanged.disconnect(volumeChanged);
    }

    Component.onCompleted: {
        volumeSlider.value = root.videoSource.volume * 100;
    }

}
