import QtQuick 2.12

Item {
    id: root
    visible: !mainViewModel.isSmallSizeMode && !onlinePlayerViewModel.showedDropWarning && onlinePlayerViewModel.isReleaseLess2022
    width: 460
    height: 200

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
        text: "<b>Релизы младше 2022 года могут содержать микродропы при воспроизведении.</b><br />
            Варианты как можно смотреть еще:<br />
            - Смотреть через браузер, первая кнопка справа от глазика в плейлисте<br />
            - Смотреть через внешний плеер, вторая кнопка справа от глазика в плейлисте<br />
            - Смотреть торрент, одноименная кнопка в карточке релиза<br />
            - Ну или просто скачать торрент, кнопка Скачать находиться тоже в карточке релиза"
    }

    IconButton {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 4
        anchors.topMargin: 4
        iconPath: assetsLocation.iconsPath + "close.svg"
        iconWidth: 20
        iconHeight: 20
        iconColor: applicationThemeViewModel.plainTextColor
        width: 22
        height: 22
        onButtonPressed: {
            root.visible = false;
            onlinePlayerViewModel.showedDropWarning = true;
        }
    }
}
