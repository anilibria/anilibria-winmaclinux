import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0
import "../Controls"

Page {
    id: root
    anchors.fill: parent
    background: Rectangle {
        color: applicationThemeViewModel.pageBackground
    }

    signal navigateFrom()
    signal navigateTo()

    RowLayout {
        id: panelContainer
        anchors.fill: parent
        spacing: 0
        Rectangle {
            color: applicationThemeViewModel.pageVerticalPanel
            width: 40
            Layout.fillHeight: true
            Column {
                IconButton {
                    height: 45
                    width: 40
                    overlayVisible: false
                    iconPath: applicationThemeViewModel.iconMainMenu
                    iconWidth: 28
                    iconHeight: 28
                    onButtonPressed: {
                        drawer.open();
                    }
                }
            }
        }
        Rectangle {
            color: "transparent"
            Layout.fillHeight: true
            Layout.fillWidth: true

            Rectangle {
                id: upperPanelContainer
                width: parent.width
                height: 45
                color: applicationThemeViewModel.pageUpperPanel

                PlainText {
                    id: statusText
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    fontPointSize: mainViewModel.isSmallSizeMode ? 10 : 12
                    text: torrentNotifierViewModel.activated ? "TorrentStream подключен" : "TorrentStream не подключен"
                }
                RoundedActionButton {
                    id: connectButton
                    visible: !torrentNotifierViewModel.activated
                    anchors.left: statusText.right
                    text: "Подключиться"
                    onClicked: {
                        torrentNotifierViewModel.startGetNotifiers(userConfigurationViewModel.playerBuffer);
                    }
                }
            }
        }
    }

}
