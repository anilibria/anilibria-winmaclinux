import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import "../Controls"
import "../Theme"

Page {
    id: myAnilibriaPage
    anchors.fill: parent

    signal navigateFrom()
    signal navigateTo()

    background: Rectangle {
        color: ApplicationTheme.pageBackground
    }

    Loader {
        anchors.fill: parent
        sourceComponent: myAnilibriaPage.visible ? myAnilibriaContent : null
    }

    Component {
        id: myAnilibriaContent

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
                        iconPath: assetsLocation.iconsPath + "menu.svg"
                        iconWidth: 29
                        iconHeight: 29
                        onButtonPressed: {
                            drawer.open();
                        }
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
                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Rectangle {
                        width: parent.width
                        height: 200
                        color: "transparent"
                        border.color: "white"
                        border.width: 1

                        AccentText {
                            id: statisticsHeaderText
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.leftMargin: 6
                            anchors.topMargin: 6
                            fontPointSize: 12
                            text: "Статистика"
                        }

                        Item {
                            id: middleSeparator
                            width: 1
                            height: parent.height
                            x: parent.width / 2
                            y: 0
                        }

                        PlainText {
                            id: watchUsingApplicationText
                            anchors.top: statisticsHeaderText.bottom
                            anchors.left: parent.left
                            anchors.leftMargin: 6
                            anchors.topMargin: 6
                            fontPointSize: 11
                            text: "Общее время в приложении: "
                        }

                        PlainText {
                            id: watchUsingApplicationTextValue
                            anchors.top: statisticsHeaderText.bottom
                            anchors.right: middleSeparator.right
                            anchors.topMargin: 6
                            fontPointSize: 11
                            text: userActivityViewModel.watchUsingApplication
                        }

                        PlainText {
                            id: watchDurationText
                            anchors.top: watchUsingApplicationText.bottom
                            anchors.left: parent.left
                            anchors.leftMargin: 6
                            anchors.topMargin: 6
                            fontPointSize: 11
                            text: "Общее время просмотра: "
                        }

                        PlainText {
                            id: watchDurationTextValue
                            anchors.top: watchUsingApplicationText.bottom
                            anchors.right: middleSeparator.right
                            anchors.leftMargin: 6
                            anchors.topMargin: 6
                            fontPointSize: 11
                            text: userActivityViewModel.watchDuration
                        }

                        PlainText {
                            id: countOpenedReleaseCardText
                            anchors.top: watchDurationText.bottom
                            anchors.left: parent.left
                            anchors.leftMargin: 6
                            anchors.topMargin: 6
                            fontPointSize: 11
                            text: "Сколько раз открывалась карточка релиза: "
                        }

                        PlainText {
                            id: countOpenedReleaseCardTextValue
                            anchors.top: watchDurationText.bottom
                            anchors.right: middleSeparator.right
                            anchors.leftMargin: 6
                            anchors.topMargin: 6
                            fontPointSize: 11
                            text: userActivityViewModel.countOpenedReleaseCard
                        }

                        PlainText {
                            id: countDownloadTorrentText
                            anchors.top: countOpenedReleaseCardText.bottom
                            anchors.left: parent.left
                            anchors.leftMargin: 6
                            anchors.topMargin: 6
                            fontPointSize: 11
                            text: "Сколько раз скачивался торрент: "
                        }

                        PlainText {
                            id: countDownloadTorrentTextValue
                            anchors.top: countOpenedReleaseCardText.bottom
                            anchors.right: middleSeparator.right
                            anchors.leftMargin: 6
                            anchors.topMargin: 6
                            fontPointSize: 11
                            text: userActivityViewModel.countDownloadTorrent
                        }

                        PlainText {
                            id: countAddedToCinemahallText
                            anchors.top: countDownloadTorrentText.bottom
                            anchors.left: parent.left
                            anchors.leftMargin: 6
                            anchors.topMargin: 6
                            fontPointSize: 11
                            text: "Сколько раз релизы доблялись в кинозал: "
                        }

                        PlainText {
                            id: countAddedToCinemahallTextValue
                            anchors.top: countDownloadTorrentText.bottom
                            anchors.right: middleSeparator.right
                            anchors.leftMargin: 6
                            anchors.topMargin: 6
                            fontPointSize: 11
                            text: userActivityViewModel.countAddedToCinemahall
                        }

                        PlainText {
                            id: mostGenresText
                            anchors.top: countAddedToCinemahallText.bottom
                            anchors.left: parent.left
                            anchors.leftMargin: 6
                            anchors.topMargin: 6
                            fontPointSize: 11
                            text: "Наиболее просматриваемые жанры: "
                        }

                        PlainText {
                            id: mostGenresTextValue
                            anchors.top: countAddedToCinemahallText.bottom
                            anchors.right: middleSeparator.right
                            anchors.leftMargin: 6
                            anchors.topMargin: 6
                            fontPointSize: 11
                            text: myAnilibriaViewModel.genres
                        }
                    }
                }
            }
        }
    }
}

