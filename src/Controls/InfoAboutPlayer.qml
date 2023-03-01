import QtQuick 2.12

Item {
    id: root
    visible: !mainViewModel.isSmallSizeMode && !onlinePlayerViewModel.showedDropWarning &&
        onlinePlayerViewModel.isReleaseLess2022 &&
        onlinePlayerWindowViewModel.isSelectedVlc
    width: 460
    height: 140

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
        text: "<b>Релизы озвученные младше 2022 года могут содержать микродропы при воспроизведении на плеере VLC</b><br />" +
            (Qt.platform.os === 'windows' ? "Чтобы решить эту проблему Вы можете смотреть на плеере QtAv с включенным проксированием на странице TorrentStream." :
                "Чтобы решить эту проблему Вы можете смотреть на плеере Default или Default classic.")
    }

    IconButton {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 4
        anchors.topMargin: 4
        iconPath: applicationThemeViewModel.currentItems.iconCloseWindow
        iconWidth: 20
        iconHeight: 20
        width: 22
        height: 22
        onButtonPressed: {
            root.visible = false;
            onlinePlayerViewModel.showedDropWarning = true;
        }
    }
}
