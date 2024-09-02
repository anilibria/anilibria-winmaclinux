import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import "../Controls"

ColumnLayout {
    id: cardContainer
    visible: extensionsViewModel.isOpenedCard
    anchors.fill: parent
    spacing: 0
    Rectangle {
        color: applicationThemeViewModel.pageBackground
        Layout.fillWidth: true
        Layout.fillHeight: true

        Rectangle {
            width: 42
            height: parent.height
            anchors.right: parent.right
            color: applicationThemeViewModel.pageUpperPanel
        }

        Column {
            id: cardButtons
            width: 42
            anchors.right: parent.right
        }

        ListView {
            id: mainScrollArea
            height: parent.height
            width: parent.width - cardButtons.width
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            model: extensionsViewModel.globalVariables
            delegate: Item {
                id: itemRoot
                width: extensionsListView.width
                height: 120

                property string modelIndentifier: modelData.indentifier

                Rectangle {
                    id: itemContainer
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: extensionsListView.width - 10
                    height: parent.height - 10
                    radius: 10
                    color: applicationThemeViewModel.currentItems.panelBackground
                }

                PlainText {
                    width: itemContainer.width - 80
                    height: itemContainer.height - 10
                    fontPointSize: 11
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    maximumLineCount: 4
                    text: modelData.indentifier + "\n" + modelData.value
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

                            CommonMenuItem {
                                text: 'Изменить'
                                onPressed: {
                                    extensionsContextMenu.close();
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
                        tooltipMessage: "Удалить переменную"
                        onButtonPressed: {
                            extensionsViewModel.deleteGlobalVariable(modelData.indentifier);
                        }
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {
                active: true
            }
        }
    }
}
