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

            IconButton {
                height: 40
                width: 40
                hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                iconPath: applicationThemeViewModel.currentItems.iconMyAnilibriaButtonPlus
                iconWidth: 26
                iconHeight: 26
                tooltipMessage: "Добавить глобальную переменную"
                onButtonPressed: {
                    editVariablePopup.open();
                }
            }
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

                                    editVariablePopup.open();
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

    DefaultPopup {
        id: editVariablePopup
        x: (window.width / 2) - (editVariablePopup.width / 2)
        y: (window.height / 2) - (editVariablePopup.height / 2)
        width: mainViewModel.isSmallSizeMode ? 250 : 390
        height: 150
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        CommonTextField {
            id: variableNameTextField
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 4
            width: parent.width - 15
            placeholderText: "Имя переменной"
        }

        CommonTextField {
            id: variableValueTextField
            anchors.top: variableNameTextField.bottom
            anchors.left: parent.left
            anchors.leftMargin: 4
            width: parent.width - 15
            placeholderText: "Значение"
        }

        RoundedActionButton {
            anchors.right: cancelButton.left
            anchors.bottom: parent.bottom
            text: "Сохранить"
            onClicked: {

                editVariablePopup.close();
            }
        }

        RoundedActionButton {
            id: cancelButton
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            text: "Отмена"
            onClicked: {
                editVariablePopup.close();
            }
        }
    }

}
