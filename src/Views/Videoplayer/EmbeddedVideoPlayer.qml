import QtQuick 2.15
import QtQuick.Controls 2.15
import "../../Controls"

Rectangle {
    id: embeddedVideoPlayer
    visible: onlinePlayerViewModel.showEmbeddedVideoWindow
    width: 300
    height: 200
    color: applicationThemeViewModel.currentItems.pageBackground
    border.width: 1
    border.color: "white"

    Loader {
        id: shaderLoader
        anchors.fill: parent
        sourceComponent: embeddedVideoPlayer.visible ? shaderLoaderComponent : null
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
            paused: !embeddedVideoPlayer.visible
            playing: embeddedVideoPlayer.visible && onlinePlayerViewModel.isBuffering
            source: assetsLocation.path + "Icons/spinner.gif"
        }
    }

    MouseArea {
        id: videoMouseArea
        enabled: embeddedVideoPlayer.visible
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        property real originX: -1
        property real originY: -1
        property bool moving: false

        onPressed: {
            if (mouse.button === Qt.LeftButton) {
                videoMouseArea.moving = true;
            }
            if (mouse.button === Qt.RightButton) {
                onlinePlayerViewModel.showEmbeddedVideoWindowPanel = !onlinePlayerViewModel.showEmbeddedVideoWindowPanel;
            }
        }
        onDoubleClicked: {
            if (mouse.button === Qt.LeftButton) {
                mainViewModel.selectPage("videoplayer");
                onlinePlayerViewModel.showEmbeddedVideoWindow = false;
            }
        }
        onReleased: {
            videoMouseArea.moving = false;
            videoMouseArea.originX = -1;
            videoMouseArea.originY = -1;
        }
        onPositionChanged: {
            if (!videoMouseArea.moving) return;

            if (originX === -1) originX = mouse.x;
            if (originY === -1) originY = mouse.y;

            const deltaX = originX - mouse.x;
            const deltaY = originY - mouse.y;

            let newXValue = embeddedVideoPlayer.x - deltaX;
            if (newXValue < 0) newXValue = 0;
            if (newXValue + videoMouseArea.width > window.width) newXValue = window.width - videoMouseArea.width;
            embeddedVideoPlayer.x = newXValue;

            let newYValue = embeddedVideoPlayer.y - deltaY;
            if (newYValue < 0) newYValue = 0;
            embeddedVideoPlayer.y = newYValue;
        }
    }

    Item {
        visible: onlinePlayerViewModel.showEmbeddedVideoWindowPanel
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: 80
        height: 40

        Rectangle {
            anchors.fill: parent
            color: applicationThemeViewModel.currentItems.pageBackground
        }

        Slider {
            id: volumeSlider
            anchors.centerIn: parent
            width: parent.width
            height: 40
            from: 0
            value: 0
            to: 100
            onMoved: {
                videoplayer.videoOrigin.volume = value;
            }
        }
    }

    Item {
        visible: onlinePlayerViewModel.showEmbeddedVideoWindowPanel
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: 120
        height: 40

        Rectangle {
            anchors.fill: parent
            color: applicationThemeViewModel.currentItems.pageBackground
        }

        Row {
            IconButton {
                id: previousButton
                width: 40
                height: 40
                hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                iconPath: applicationThemeViewModel.currentItems.iconPlayerPreviousSeria
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
                hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                iconPath: applicationThemeViewModel.currentItems.iconPlayerPlay
                iconWidth: 24
                iconHeight: 24
                onButtonPressed: {
                    onlinePlayerViewModel.playInPlayer();
                }
            }
            IconButton {
                id: pauseButton
                visible: onlinePlayerWindowViewModel.pauseButtonVisible
                width: 40
                height: 40
                hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                iconPath: applicationThemeViewModel.currentItems.iconPlayerPause
                iconWidth: 24
                iconHeight: 24
                onButtonPressed: {
                    onlinePlayerViewModel.pauseInPlayer();
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
                tooltipMessage: "Следующая серия"
            }
        }
    }

    Component {
        id: shaderLoaderComponent

        ShaderEffectSource {
            anchors.fill: parent
            anchors.margins: 1
            sourceItem: onlinePlayerWindowViewModel.isSelectedMpv ? videoplayer.videoPlayerSource : videoplayer.videoOutputSource
        }

    }

    onVisibleChanged: {
        if (visible === false) {
            videoMouseArea.moving = false;
            videoMouseArea.originX = -1;
            videoMouseArea.originY = -1;
            onlinePlayerViewModel.showEmbeddedVideoWindowPanel = false;
        }
        if (visible === true) {
            volumeSlider.value = videoplayer.videoOrigin.volume;
        }
    }
}
