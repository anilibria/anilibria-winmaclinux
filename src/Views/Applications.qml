import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import "../Controls"

Page {
    id: rootPage
    anchors.fill: parent
    property int page: 0
    property var allItemsPage

    signal navigateFrom()
    signal navigateTo()

    background: Rectangle {
        color: applicationThemeViewModel.pageBackground
    }

    Loader {
        anchors.fill: parent
        sourceComponent: rootPage.visible ? applicationPageContent : null
    }

    Component {
        id: applicationPageContent

        Item {
            anchors.fill: parent

            Rectangle {
                id: mask
                width: 472
                height: 240
                radius: 10
                visible: false
            }

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
                        color: applicationThemeViewModel.pageUpperPanel
                    }

                    ListView {
                        id: scrollView
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        model: applicationsViewModel.items
                        clip: true
                        spacing: 4
                        ScrollBar.vertical: ScrollBar {
                            active: true
                        }
                        delegate: Rectangle {
                            width: scrollView.width
                            height: 170
                            color: "transparent"

                            Rectangle {
                                anchors.fill: parent
                                anchors.leftMargin: 4
                                anchors.rightMargin: 4
                                radius: 10
                                color: applicationThemeViewModel.panelBackground

                                /*MouseArea {
                                    anchors.fill: parent
                                    onPressed: {
                                        releaseLinkedSeries.selectByIndex(identifier);
                                        mainViewModel.selectPage("releaseseries:" + identifier);
                                    }
                                }*/

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.leftMargin: 4

                                    Rectangle {
                                        height: parent.height
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        color: "transparent"

                                        ColumnLayout {
                                            spacing: 2
                                            anchors.fill: parent
                                            anchors.leftMargin: 10

                                            Item {
                                                Layout.fillHeight: true
                                            }

                                            AccentText {
                                                fontPointSize: 10
                                                text: modelData.applicationName
                                                Layout.fillWidth: true
                                                maximumLineCount: 2
                                                elide: Text.ElideRight
                                                wrapMode: Text.Wrap
                                            }

                                            PlainText {
                                                Layout.topMargin: 8
                                                Layout.bottomMargin: 4
                                                Layout.fillWidth: true
                                                Layout.rightMargin: 30
                                                fontPointSize: 8
                                                text: modelData.description
                                                maximumLineCount: 2
                                                elide: Text.ElideRight
                                                wrapMode: Text.Wrap
                                            }

                                            PlainText {
                                                Layout.topMargin: 2
                                                fontPointSize: 9
                                                text: modelData.isInstalled ? "Установлено" : "Не установлено"
                                            }

                                            PlainText {
                                                Layout.topMargin: 2
                                                fontPointSize: 9
                                                visible: modelData.isHaveNewVersion
                                                text: modelData.isHaveNewVersion ? "Можно установить новую версию!" : ""
                                            }

                                            PlainText {
                                                Layout.topMargin: 2
                                                fontPointSize: 9
                                                visible: modelData.includedInDistributive
                                                text: "Является частью дистрибутива и не требует установки"
                                            }

                                            PlainText {
                                                Layout.topMargin: 2
                                                Layout.fillWidth: true
                                                fontPointSize: 9
                                                visible: modelData.isInstalled && !modelData.includedInDistributive
                                                text: "Путь установки: " + modelData.installedPath
                                                maximumLineCount: 2
                                                elide: Text.ElideRight
                                                wrapMode: Text.Wrap
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
                                                visible: !modelData.isInstalled || modelData.isHaveNewVersion
                                                iconPath: applicationThemeViewModel.currentItems.iconFavorites
                                                tooltipMessage: !modelData.isInstalled ? "Установить приложение" : "Обновить приложение"
                                                onButtonPressed: {

                                                }
                                            }
                                            FilterPanelIconButton {
                                                visible: modelData.isInstalled && !modelData.includedInDistributive
                                                iconPath: applicationThemeViewModel.currentItems.iconDeleteItem
                                                tooltipMessage: "Удалить приложение"
                                                onButtonPressed: {

                                                }                                                
                                            }
                                            /*FilterPanelIconButton {
                                                iconPath: applicationThemeViewModel.currentItems.iconMainMenuVideoplayer
                                                tooltipMessage: "Восстановить приложение"
                                                onButtonPressed: {

                                                }
                                            }*/
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

