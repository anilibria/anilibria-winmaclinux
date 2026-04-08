import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Anilibria.ViewModels 1.0
import "../Controls"
import "Videoplayer"

ApplicationWindow {
    id: root
    title: '  '
    width: 450
    height: 150
    flags: Qt.Dialog | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint
    minimumWidth: 400
    minimumHeight: 150
    maximumWidth: 450
    maximumHeight: 150

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
                anchors.topMargin: 2
                anchors.horizontalCenter: parent.horizontalCenter
                fontPointSize: 9
                text: "Серия " + externalPlayerViewModel.currentSeria
            }

            PlainText {
                id: statusLabel
                anchors.top: seriaLabel.bottom
                anchors.topMargin: 2
                anchors.horizontalCenter: parent.horizontalCenter
                fontPointSize: 8
                text: externalPlayerViewModel.status ? externalPlayerViewModel.status : externalPlayerViewModel.position
            }
        }

        RowLayout {
            anchors.top: releaseNameItem.bottom
            width: parent.width
            height: parent.height - releaseNameItem.height

            Rectangle {
                id: leftPart
                color: "transparent"
                Layout.preferredWidth: 80
                Layout.fillHeight: true

                Slider {
                    id: volumeSlider
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 4
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
                    width: previousButton.width + playButton.width + nextButton.width
                    height: parent.height
                    anchors.horizontalCenter: parent.horizontalCenter

                    IconButton {
                        id: previousButton
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 4
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
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 4
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
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 4
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
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 4
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
                Layout.preferredWidth: 80
                Layout.fillHeight: true

                IconButton {
                    anchors.right: topmostButton.left
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 4
                    width: 40
                    height: 40
                    hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                    iconPath: externalPlayerViewModel.muted ? applicationThemeViewModel.currentItems.iconPlayerMute : applicationThemeViewModel.currentItems.iconPlayerUnMute
                    iconWidth: 24
                    iconHeight: 24
                    onButtonPressed: {
                        const newstate = !externalPlayerViewModel.muted;
                        externalPlayerViewModel.changeMute(newstate);
                    }
                }

                IconButton {
                    id: topmostButton
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 4
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
