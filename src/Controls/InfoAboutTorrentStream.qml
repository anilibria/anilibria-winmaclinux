import QtQuick 2.15

Item {
    id: root
    visible: synchronizationServicev2.notInstalledTorrentStream
    enabled: synchronizationServicev2.notInstalledTorrentStream
    width: 320
    height: 120

    Rectangle {
        anchors.fill: parent
        color: applicationThemeViewModel.panelBackground
        border.width: 1
        border.color: applicationThemeViewModel.plainTextColor
        radius: 8
    }

    LinkedText {
        anchors.fill: parent
        anchors.margins: 4
        wrapMode: Text.Wrap
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        fontPointSize: 10
        text: "<b>Библиотека TorrentStream устанавливается...</b><br />Она нужна для того чтобы исправлять проглатывания видео в плеере а также скачивать торренты"
    }

    Component.onCompleted: {
        if (!userConfigurationViewModel.showedVideoForNewcomers) root.visible = true;
    }
}
