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
                        id: statisticsHeader
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.margins: 4
                        height: 30
                        color: "transparent"
                        border.color: "white"
                        border.width: 1
                        radius: 4

                        AccentText {
                            anchors.left: parent.left
                            anchors.leftMargin: 4
                            anchors.verticalCenter: parent.verticalCenter
                            text: "Статистика"
                            fontPointSize: 12
                        }

                        IconButton {
                            width: 26
                            height: parent.height - 4
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            anchors.rightMargin: 4
                            iconColor: "white"
                            iconWidth: 22
                            iconHeight: 22
                            iconPath: assetsLocation.iconsPath + "arrowup.svg"
                            transform: Rotation {
                                angle: userConfigurationViewModel.hideStatistics ? 180 : 0
                                origin.x: 13
                                origin.y: 13
                            }
                            onButtonPressed: {
                                userConfigurationViewModel.hideStatistics = !userConfigurationViewModel.hideStatistics;
                            }
                        }
                    }

                    Item {
                        anchors.top: statisticsHeader.bottom
                        anchors.topMargin: 10
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        width: parent.width
                        height: userConfigurationViewModel.hideStatistics ? 0 : 200
                        visible: !userConfigurationViewModel.hideStatistics

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

                        PlainText {
                            id: mostVoicesText
                            anchors.top: mostGenresText.bottom
                            anchors.left: parent.left
                            anchors.leftMargin: 6
                            anchors.topMargin: 6
                            fontPointSize: 11
                            text: "Наиболее просматриваемые войсеры: "
                        }

                        PlainText {
                            id: mostVoicesTextValue
                            anchors.top: mostGenresText.bottom
                            anchors.right: middleSeparator.right
                            anchors.leftMargin: 6
                            anchors.topMargin: 6
                            fontPointSize: 11
                            text: myAnilibriaViewModel.voices
                        }

                    }
                }
            }
        }
    }
}

