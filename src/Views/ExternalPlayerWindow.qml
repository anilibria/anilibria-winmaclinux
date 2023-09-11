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
    width: 450
    height: 250
    flags: Qt.Dialog | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint
    minimumWidth: 350
    minimumHeight: 200
    maximumWidth: 500
    maximumHeight: 350

    signal showWindow()
    signal closeWindow()

    background: Rectangle {
        color: applicationThemeViewModel.pageBackground
    }

    Item {
        anchors.fill: parent

        Item {
            id: releaseNameItem
            width: parent.width
            height: 60

            AccentText {
                id: headerLabel
                anchors.centerIn: parent
                text: externalPlayerViewModel.releaseName
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                maximumLineCount: 2
                fontPointSize: 10
            }

            PlainText {
                id: seriaLabel
                anchors.top: headerLabel.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                fontPointSize: 9
                text: "Серия " + externalPlayerViewModel.currentSeria
            }

            PlainText {
                anchors.top: seriaLabel.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                fontPointSize: 8
                text: externalPlayerViewModel.status
            }
        }

        RowLayout {
            anchors.top: releaseNameItem.bottom
            width: parent.width
            height: parent.height - releaseNameItem.height

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
        externalPlayerViewModel.closePlayer();
        hide();
    }
}
