import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtMultimedia 5.12
import Anilibria.ViewModels 1.0
import "../Controls"
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

    signal showWindow()
    signal closeWindow()

    Rectangle {
        id: controlPanel
        color: applicationThemeViewModel.playerControlBackground
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
                    value: externalPlayerViewModel.volume
                    to: 100
                    onPressedChanged: {
                        controlPanel.forceActiveFocus();
                    }
                    onMoved: {
                        externalPlayerViewModel.changeVolume(value);
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
                        hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                        iconPath: applicationThemeViewModel.currentItems.iconPlayerPreviousSeria
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            externalPlayerViewModel.previousVideo();
                        }
                        tooltipMessage: "Предыдущая серия"
                    }

                    IconButton {
                        id: playButton
                        visible: externalPlayerViewModel.isPaused || externalPlayerViewModel.isStopped
                        width: 40
                        height: 40
                        hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                        iconPath: applicationThemeViewModel.currentItems.iconPlayerPlay
                        iconWidth: 24
                        iconHeight: 24
                        tooltipMessage: "Воспроизведение"
                        onButtonPressed: {
                            externalPlayerViewModel.play();
                        }
                    }
                    IconButton {
                        id: pauseButton
                        visible: externalPlayerViewModel.isPlaying
                        width: 40
                        height: 40
                        hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                        iconPath: applicationThemeViewModel.currentItems.iconPlayerPause
                        iconWidth: 24
                        iconHeight: 24
                        tooltipMessage: "Пауза"
                        onButtonPressed: {
                            externalPlayerViewModel.pause();
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
                            externalPlayerViewModel.nextVideo();
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
                    hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                    iconPath: applicationThemeViewModel.currentItems.iconPlayerTopMost
                    iconWidth: 29
                    iconHeight: 29
                    tooltipMessage: ""
                    onButtonPressed: {
                    }
                }
            }
        }

        Behavior on opacity {
            NumberAnimation { duration: 200 }
        }
    }

    /*Rectangle {
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
    }*/

    onShowWindow:  {
        show();
    }

    onClosing: {
        closeWindow();
    }

    onCloseWindow: {
        hide();
    }
}
