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
                        overlayVisible: false
                        iconPath: assetsLocation.iconsPath + "coloreddrawer.svg"
                        iconWidth: 40
                        iconHeight: 40
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

                Flickable {
                    id: scrollview
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    clip: true
                    contentWidth: width
                    contentHeight: pageContent.height
                    ScrollBar.vertical: ScrollBar {
                        active: true
                    }

                    Item {
                        id: pageContent
                        width: parent.width
                        height: statisticsHeader.height + (statisticsContent.visible ? statisticsContent.height : 0) +
                            (updateInFavorites.hasItems ? hideUpdatesByFavoritesHeader.height + (updateInFavorites.visible ? updateInFavorites.height : 0) : 0) +
                            (updatesFromStart.hasItems ? hideUpdatesFromStartHeader.height + (updatesFromStart.visible ? updatesFromStart.height : 0 ) : 0) +
                            (updatesLastTwoDays.hasItems ? headerUpdatesLastTwoDays.height + (updatesLastTwoDays.visible ? updatesLastTwoDays.height : 0 ) : 0) +
                            (abandonedSeens.hasItems ? headerAbandonedSeens.height + (abandonedSeens.visible ? abandonedSeens.height : 0 ) : 0) + 20

                        ExpandableHeader {
                            id: statisticsHeader
                            height: statisticsHeader.visible ? 30 : 0
                            headerTitle: "Статистика"
                            headerHided: userConfigurationViewModel.hideStatistics
                            onHeaderPressed: {
                                userConfigurationViewModel.hideStatistics = !userConfigurationViewModel.hideStatistics;
                            }
                        }

                        Item {
                            id: statisticsContent
                            anchors.top: statisticsHeader.bottom
                            anchors.topMargin: 0
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
                                anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.leftMargin: 6
                                anchors.topMargin: 6
                                fontPointSize: 11
                                text: "Общее время в приложении: "
                            }

                            PlainText {
                                id: watchUsingApplicationTextValue
                                anchors.top: parent.top
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

                        ExpandableHeader {
                            id: hideUpdatesByFavoritesHeader
                            visible: updateInFavorites.hasItems
                            height: statisticsHeader.visible ? 30 : 0
                            anchors.top: statisticsContent.bottom
                            headerTitle: "Обновления по избранному"
                            headerHided: userConfigurationViewModel.hideUpdatesByFavorites
                            onHeaderPressed: {
                                userConfigurationViewModel.hideUpdatesByFavorites = !userConfigurationViewModel.hideUpdatesByFavorites;
                            }
                        }

                        ReleasesList {
                            id: updateInFavorites
                            visible: updateInFavorites.hasItems && !userConfigurationViewModel.hideUpdatesByFavorites
                            height: updateInFavorites.visible ? 330 : 0
                            anchors.top: hideUpdatesByFavoritesHeader.bottom
                            filterMode: "newinfavorites"
                        }

                        ExpandableHeader {
                            id: hideUpdatesFromStartHeader
                            visible: updatesFromStart.hasItems
                            height: updatesFromStart.visible ? 30 : 0
                            anchors.top: updateInFavorites.bottom
                            headerTitle: "Обновления с последнего посещения"
                            headerHided: userConfigurationViewModel.hideUpdatesFromStart
                            onHeaderPressed: {
                                userConfigurationViewModel.hideUpdatesFromStart = !userConfigurationViewModel.hideUpdatesFromStart;
                            }
                        }

                        ReleasesList {
                            id: updatesFromStart
                            visible: updatesFromStart.hasItems && !userConfigurationViewModel.hideUpdatesFromStart
                            height: updatesFromStart.visible ? 330 : 0
                            anchors.top: hideUpdatesFromStartHeader.bottom
                            filterMode: "newfromstart"
                        }

                        ExpandableHeader {
                            id: headerUpdatesLastTwoDays
                            visible: updatesLastTwoDays.hasItems
                            height: headerUpdatesLastTwoDays.visible ? 30 : 0
                            anchors.top: updatesFromStart.bottom
                            headerTitle: "Последние обновления"
                            headerHided: userConfigurationViewModel.hideUpdatesLastTwoDays
                            onHeaderPressed: {
                                userConfigurationViewModel.hideUpdatesLastTwoDays = !userConfigurationViewModel.hideUpdatesLastTwoDays;
                            }
                        }

                        ReleasesList {
                            id: updatesLastTwoDays
                            visible: updatesLastTwoDays.hasItems && !userConfigurationViewModel.hideUpdatesLastTwoDays
                            height: updatesLastTwoDays.visible ? 330 : 0
                            anchors.top: headerUpdatesLastTwoDays.bottom
                            filterMode: "lasttwodays"
                        }

                        ExpandableHeader {
                            id: headerAbandonedSeens
                            visible: abandonedSeens.hasItems
                            height: headerAbandonedSeens.visible ? 30 : 0
                            anchors.top: updatesLastTwoDays.bottom
                            headerTitle: "Брошенный просмотр"
                            headerHided: userConfigurationViewModel.hideAbandonedSeens
                            onHeaderPressed: {
                                userConfigurationViewModel.hideAbandonedSeens = !userConfigurationViewModel.hideAbandonedSeens;
                            }
                        }

                        ReleasesList {
                            id: abandonedSeens
                            visible: updatesLastTwoDays.hasItems && !userConfigurationViewModel.hideAbandonedSeens
                            height: updatesLastTwoDays.visible ? 330 : 0
                            anchors.top: headerAbandonedSeens.bottom
                            filterMode: "abondonedseens"
                        }
                    }
                }
            }
        }
    }
}

