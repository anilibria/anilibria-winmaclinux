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
        //mainGrid.model.clearCharacters();
        //releasesViewModel.items.alphabetsFilter = "";
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
                width: 400
                height: 300
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

                    RoundedActionButton {
                        id: saveOrEditButton
                        width: 180
                        text: releasesViewModel.customGroups.addGroupEditMode ? "Сохранить группу" : "Добавить группу"
                        anchors.left: parent.left
                        anchors.leftMargin: 4
                        onClicked: {
                            if (releasesViewModel.customGroups.addGroupEditMode) {
                                if (groupNameTextField.text) {
                                    releasesViewModel.customGroups.addGroup(groupNameTextField.text);
                                    groupNameTextField.text = "";
                                }
                            } else {
                                releasesViewModel.customGroups.startEditNewGroup();
                            }
                        }
                    }

                    CommonTextField {
                        id: groupNameTextField
                        visible: releasesViewModel.customGroups.addGroupEditMode
                        width: 180
                        anchors.left: saveOrEditButton.right
                    }
                }

                ListView {
                    visible: releasesViewModel.customGroups.hasGroups
                    anchors.fill: parent
                    anchors.topMargin: 44
                    anchors.leftMargin: 4
                    anchors.rightMargin: 4
                    anchors.bottomMargin: 4
                    clip: true
                    model: releasesViewModel.customGroups.groups
                    delegate: Item {
                        width: ListView.view.width
                        height: 20

                        PlainText {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.leftMargin: 10
                            anchors.rightMargin: 10
                            text: modelData.groupName
                            fontPointSize: 10
                            maximumLineCount: 1
                            elide: Text.ElideRight
                        }

                        MouseArea {
                            anchors.fill: parent
                            onPressed: {
                                console.log(modelData.index);
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
