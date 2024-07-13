import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../Controls"

Page {
    id: root
    anchors.fill: parent

    background: Rectangle {
        color: applicationThemeViewModel.currentItems.pageBackground
    }

    Loader {
        anchors.fill: parent
        sourceComponent: youtubePage.visible ? youtubePageContent : null
    }

    Component {
        id: extensionsContent

        Item {
            anchors.fill: parent

            RowLayout {
                id: panelContainer
                anchors.fill: parent
                spacing: 0

                ColumnLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    spacing: 2

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 45
                        height: 45
                        color: applicationThemeViewModel.currentItems.pageUpperPanel
                    }

                    ListView {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        model: releaseLinkedSeries
                        clip: true
                        spacing: 4
                        ScrollBar.vertical: ScrollBar {
                            active: true
                        }
                        delegate: Rectangle {
                            width: itemContainer.width
                            height: 220
                            color: "transparent"

                            Rectangle {
                                anchors.fill: parent
                                anchors.leftMargin: 4
                                anchors.rightMargin: 4
                                radius: 10
                                color: applicationThemeViewModel.panelBackground

                                MouseArea {
                                    anchors.fill: parent
                                    onPressed: {
                                        /*releaseLinkedSeries.selectByIndex(identifier);
                                        mainViewModel.selectPage("releaseseries:" + identifier);*/
                                    }
                                }

                                RowLayout {
                                    anchors.fill: parent

                                    Rectangle {
                                        height: parent.height
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        color: "transparent"

                                        ColumnLayout {
                                            spacing: 4
                                            anchors.fill: parent
                                            anchors.leftMargin: 10

                                            Item {
                                                Layout.fillHeight: true
                                            }

                                            AccentText {
                                                fontPointSize: 10
                                                text: firstThreeNamesRole
                                                Layout.fillWidth: true
                                                maximumLineCount: 2
                                                elide: Text.ElideRight
                                                wrapMode: Text.Wrap
                                            }

                                            PlainText {
                                                Layout.preferredHeight: 20
                                                fontPointSize: 10
                                                visible: !!otherReleases
                                                text: otherReleases
                                            }

                                            PlainText {
                                                Layout.preferredHeight: 20
                                                Layout.fillWidth: true
                                                maximumLineCount: 2
                                                elide: Text.ElideRight
                                                wrapMode: Text.Wrap
                                                fontPointSize: 10
                                                text: genres
                                            }

                                            PlainText {
                                                Layout.preferredHeight: 20
                                                Layout.fillWidth: true
                                                maximumLineCount: 2
                                                elide: Text.ElideRight
                                                wrapMode: Text.Wrap
                                                fontPointSize: 9
                                                text: countInFavorites
                                            }

                                            Item {
                                                Layout.fillHeight: true
                                            }
                                        }
                                    }
                                    Rectangle {
                                        id: rightBlock
                                        Layout.preferredWidth: 200
                                        Layout.fillHeight: true
                                        Layout.rightMargin: 6
                                        height: parent.height
                                        color: "transparent"

                                        Column {
                                            anchors.centerIn: parent

                                            FilterPanelIconButton {
                                                iconPath: applicationThemeViewModel.currentItems.iconMainMenuCinemahall
                                                tooltipMessage: "Открыть меню операций"
                                                onButtonPressed: {
                                                    commandsMenu.open();
                                                }

                                                CommonMenu {
                                                    id: commandsMenu
                                                    y: parent.y
                                                    width: 300

                                                    CommonMenuItem {
                                                        text: "Выполнить команду"
                                                        onPressed: {

                                                            commandsMenu.close();
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Rectangle {
                color: "transparent"
                width: 190
                height: 50
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.bottom: parent.bottom

                LeftPanelIconButton {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.topMargin: 8
                    hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                    tooltipMessage: "Вернуться в начало списка релизов"
                    visible: scrollView.contentY > 100
                    iconPath: applicationThemeViewModel.currentItems.iconBackToTop
                    iconWidth: 24
                    iconHeight: 24
                    onButtonPressed: {
                        scrollView.contentY = 0;
                    }

                }
            }
        }
    }
}
