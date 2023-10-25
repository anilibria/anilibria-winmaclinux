import QtQuick 2.12
import QtQuick.Layouts 1.3
import Anilibria.ListModels 1.0
import "../Controls"

Item {
    id: root
    anchors.fill: parent
    visible: releasesViewModel.customGroups.visible

    signal clearFilters()

    onClearFilters: {
        releasesViewModel.customGroups.clearFilters();
    }

    Loader {
        anchors.fill: parent
        sourceComponent: root.visible ? customGroupComponent : null
    }

    Component {
        id: customGroupComponent

        Item {
            anchors.fill: parent

            MouseArea {
                anchors.fill: parent
                onPressed: {
                    releasesViewModel.customGroups.visible = false;
                }
            }

            Rectangle {
                color: applicationThemeViewModel.currentItems.colorDrawerDivider
                anchors.fill: parent
            }

            Item {
                id: panelContainer
                width: mainViewModel.isSmallSizeMode ? 300 : 400
                height: mainViewModel.isSmallSizeMode ? 200 : 300
                anchors.centerIn: parent

                Rectangle {
                    anchors.left: parent.left
                    anchors.leftMargin: 2
                    anchors.top: parent.top
                    anchors.topMargin: 2
                    width: parent.width
                    height: parent.height
                    color: "transparent"
                    border.color: applicationThemeViewModel.panelBackgroundShadow
                    border.width: 2
                    radius: 10
                }

                Rectangle {
                    anchors.fill: parent
                    color: applicationThemeViewModel.currentItems.panelBackground
                    radius: 10
                }

                Item {
                    anchors.top: parent.top
                    width: parent.width
                    height: 44

                    CommonTextField {
                        id: groupNameTextField
                        visible: releasesViewModel.customGroups.groupEditMode
                        width: 180
                        anchors.right: cancelButton.left
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                    }

                    RoundedActionButton {
                        id: cancelButton
                        visible: releasesViewModel.customGroups.groupEditMode
                        width: 110
                        text: "Отмена"
                        textSize: 9
                        anchors.right: saveOrEditButton.left
                        onClicked: {
                            groupNameTextField.text = "";
                            releasesViewModel.customGroups.cancelEdit();
                        }
                    }

                    RoundedActionButton {
                        id: saveOrEditButton
                        width: 110
                        text: releasesViewModel.customGroups.groupEditMode ? "Сохранить" : "Добавить"
                        textSize: 9
                        anchors.right: parent.right
                        anchors.rightMargin: 4
                        onClicked: {
                            if (releasesViewModel.customGroups.groupEditMode) {
                                if (groupNameTextField.text) {
                                    releasesViewModel.customGroups.saveGroup(groupNameTextField.text);
                                    groupNameTextField.text = "";
                                }
                            } else {
                                releasesViewModel.customGroups.startEditNewGroup();
                            }
                        }
                    }
                }

                ListView {
                    visible: releasesViewModel.customGroups.hasGroups
                    anchors.fill: parent
                    anchors.topMargin: 44
                    anchors.leftMargin: 4
                    anchors.rightMargin: 4
                    anchors.bottomMargin: 4
                    spacing: 2
                    clip: true
                    model: releasesViewModel.customGroups.groups
                    delegate: Item {
                        width: ListView.view.width
                        height: 30

                        Rectangle {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.leftMargin: 4
                            anchors.rightMargin: 54
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            color: "transparent"
                            radius: 4
                            border.color: applicationThemeViewModel.currentItems.selectedItem
                            border.width: modelData.isSelected ? 2 : 0
                        }

                        PlainText {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.leftMargin: 10
                            anchors.rightMargin: 60
                            anchors.verticalCenter: parent.verticalCenter
                            text: modelData.groupName
                            fontPointSize: 10
                            maximumLineCount: 1
                            elide: Text.ElideRight

                            MouseArea {
                                anchors.fill: parent
                                onPressed: {
                                    const items = releasesViewModel.items;
                                    releasesViewModel.customGroups.toggleSelectGroup(modelData.index);
                                    items.refresh();
                                }
                            }
                        }

                        IconButton {
                            anchors.right: deleteGroupButton.left
                            anchors.verticalCenter: parent.verticalCenter
                            width: 20
                            height: 20
                            iconWidth: 20
                            iconHeight: 20
                            iconPath: applicationThemeViewModel.currentItems.iconEdit
                            tooltipMessage: "Изменить название"
                            onButtonPressed: {
                                if (releasesViewModel.customGroups.groupEditMode) return;

                                releasesViewModel.customGroups.startEditGroup(modelData.index);
                            }
                        }

                        IconButton {
                            id: deleteGroupButton
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            width: 20
                            height: 20
                            iconWidth: 20
                            iconHeight: 20
                            iconPath: applicationThemeViewModel.currentItems.iconDeleteItem
                            tooltipMessage: "Удалить группу"
                            onButtonPressed: {
                                releasesViewModel.customGroups.deleteGroup(modelData.index);
                            }
                        }
                    }
                }

                Item {
                    visible: !releasesViewModel.customGroups.hasGroups
                    anchors.fill: parent

                    Image {
                        id: emptyImage
                        anchors.centerIn: parent
                        width: 20
                        height: 20
                        source: applicationThemeViewModel.currentItems.iconEmpty
                        mipmap: true
                    }

                    PlainText {
                        width: parent.width - 20
                        anchors.top: emptyImage.bottom
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        fontPointSize: 10
                        textFormat: Text.RichText
                        text: "У Вас нет групп<br>Для добавления новой группы нажмите кнопку <b>Добавить группу</b> в верху этой панели."
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }
        }
    }
}
