import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import Anilibria.Services 1.0
import "../Theme"
import "../Controls"

Page {
    id: root
    anchors.fill: parent

    background: Rectangle {
        color: ApplicationTheme.pageBackground
    }

    signal navigateFrom()
    signal navigateTo()


    DownloadManager {
        id: downloadManager
        url: "https://de8.libria.fun/get/0aixBL0j9zCsrNS3P74q0Q/1590607437/mp4/8619/0008.mp4?download=Shironeko Project: Zero Chronicle-8-hd.mp4"
        destination: ""
    }

    RowLayout {
        id: panelContainer
        anchors.fill: parent
        spacing: 0
        Rectangle {
            color: ApplicationTheme.pageVerticalPanel
            width: 40
            Layout.fillHeight: true
            Column {
                IconButton {
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/menu.svg"
                    iconWidth: 29
                    iconHeight: 29
                    onButtonPressed: {
                        drawer.open();
                    }
                }
                IconButton {
                    id: startDownload
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/refresh.svg"
                    iconWidth: 34
                    iconHeight: 34
                    onButtonPressed: {
                        downloadManager.start();
                    }
                    ToolTip.delay: 1000
                    ToolTip.visible: hovered
                    ToolTip.text: "Запустить скачивание видео файлов"
                }
            }
        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 2

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 45
                height: 45
                color: ApplicationTheme.pageUpperPanel

                PlainText {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    visible: downloadManager.running
                    fontPointSize: 12
                    text: "Скачивается " + downloadManager.displayBytesInSeconds + " " + downloadManager.progress
                }
            }

            ListView {
                id: downloadReleases
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }
}