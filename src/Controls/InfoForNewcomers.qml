import QtQuick 2.15

Item {
    id: root
    visible: false
    enabled: root.visible
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
        text: "<b>Первый раз в приложении?</b><br /><br />Вы можете посмотреть <a href='https://t.me/desktopclientanilibria/259'>обучающее видео</a>.
            <br /><br />Cсылка всегда доступна в разделе помощи (кнопка с иконкой i)"
        onLinkActivated: {
            root.visible = false;
            userConfigurationViewModel.showedVideoForNewcomers = true;
        }
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
            userConfigurationViewModel.showedVideoForNewcomers = true;
        }
    }
    Component.onCompleted: {
        if (!userConfigurationViewModel.showedVideoForNewcomers) root.visible = true;
    }
}
