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
        sourceComponent: root.visible ? extensionsPageContent : null
    }

    Component {
        id: extensionsPageContent

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

                    Item {
                        id: scrollView
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        ListView {
                            id: extensionsListView
                            anchors.fill: parent
                            boundsBehavior: Flickable.StopAtBounds
                            model: extensionsViewModel.displayedExtensions
                            clip: true
                            delegate: Item {
                                id: itemRoot
                                width: extensionsListView.width
                                height: 150

                                property string modelIndentifier: modelData.indentifier

                                Rectangle {
                                    id: itemContainer
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    width: extensionsListView.width - 10
                                    height: parent.height - 10
                                    radius: 10
                                    color: applicationThemeViewModel.currentItems.panelBackground

                                    PlainText {
                                        width: itemContainer.width - 80
                                        height: itemContainer.height - 10
                                        fontPointSize: 11
                                        anchors.left: parent.left
                                        anchors.leftMargin: 10
                                        anchors.verticalCenter: parent.verticalCenter
                                        verticalAlignment: Text.AlignVCenter
                                        maximumLineCount: 4
                                        text: modelData.extensionTitle + (modelData.author ? "\n" + modelData.author : "")
                                    }

                                    Column {
                                        id: elements
                                        anchors.verticalCenter: parent.verticalCenter
                                        width: 100
                                        anchors.right: parent.right
                                        anchors.rightMargin: 10
                                        spacing: 3

                                        IconButton {
                                            anchors.right: parent.right
                                            anchors.rightMargin: 26
                                            width: 30
                                            height: 30
                                            iconWidth: 20
                                            iconHeight: 20
                                            hoverColor: applicationThemeViewModel.currentItems.filterIconButtonHoverColor
                                            iconPath: applicationThemeViewModel.currentItems.iconContextMenu
                                            onButtonPressed: {
                                                extensionsContextMenu.open();
                                            }

                                            CommonMenu {
                                                id: extensionsContextMenu
                                                autoWidth: true

                                                Repeater {
                                                    model: modelData.menuItems
                                                    delegate: CommonMenuItem {
                                                        text: modelData
                                                        onPressed: {
                                                            extensionsViewModel.runMenuCommand(itemRoot.modelIndentifier, index);
                                                            extensionsContextMenu.close();
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        IconButton {
                                            anchors.right: parent.right
                                            anchors.rightMargin: 26
                                            width: 30
                                            height: 30
                                            iconWidth: 20
                                            iconHeight: 20
                                            hoverColor: applicationThemeViewModel.currentItems.filterIconButtonHoverColor
                                            iconPath: applicationThemeViewModel.currentItems.iconDeleteItem
                                            tooltipMessage: "Удалить расширение из приложения"
                                            onButtonPressed: {
                                                extensionsViewModel.deleteExtension(itemRoot.modelIndentifier);

                                                extensionDeleteMessage.open();
                                            }

                                            MessageModal {
                                                id: extensionDeleteMessage
                                                header: "Расширение удалено"
                                                message: "Запущенное расширение будет продолжать работать до перезапуска приложения"
                                                content: Row {
                                                    spacing: 6
                                                    anchors.right: parent.right

                                                    RoundedActionButton {
                                                        text: "Закрыть"
                                                        width: 100
                                                        onClicked: {
                                                            extensionDeleteMessage.close();
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
        }
    }
}
