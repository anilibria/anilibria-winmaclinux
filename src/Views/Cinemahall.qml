import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.3
import "../Theme"
import "../Controls"

Page {
    id: root
    anchors.fill: parent

    property var selectedItems: ({})
    property int dragRelease: -1
    property var dragReleaseParent
    property bool dragActive: false
    property int dropRelease: -1

    background: Rectangle {
        color: ApplicationTheme.pageBackground
    }

    signal navigateFrom()
    signal navigateTo()

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
                    iconPath: "../Assets/Icons/menu.svg"
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

                Switch {
                    id: selectMode
                    anchors.left: parent.left
                    onCheckedChanged: {
                        root.selectedItems = {};
                    }
                    ToolTip.delay: 1000
                    ToolTip.visible: selectMode.hovered
                    ToolTip.text: "Данный переключатель влияет на поведение при клике ЛКМ на релизах в списке\nРежим выбора выбор позволяет выбрать несколько релизов и выполнять действия(например удаление)\nРежим изменения порядка позволяет изменять порядок релизов в списке путем перетаскивания\nЧтобы переключать его можно использовать клик ПКМ в области списка релизов"
                }
                PlainText {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 4
                    anchors.left: selectMode.right
                    fontPointSize: 12
                    text: selectMode.checked ? "Режим изменения порядка" : "Режим выбора"

                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            selectMode.checked = !selectMode.checked;
                        }
                    }
                }
            }

            ListModel {
                id: releasesModel
            }

            Rectangle {
                id: mask
                width: 180
                height: 260
                radius: 10
                visible: false
            }

            ListView {
                id: listViewReleases
                Layout.fillHeight: true
                Layout.fillWidth: true
                orientation: ListView.Horizontal
                model: releasesModel
                clip: true                
                delegate: Rectangle {
                    width: 280
                    height: listViewReleases.height
                    color: "transparent"

                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.RightButton
                        onClicked: {
                            selectMode.checked = !selectMode.checked;
                        }
                    }

                    Rectangle {
                        id: itemContainer
                        anchors.verticalCenter: parent.verticalCenter
                        width: 280
                        height: 280
                        radius: 10
                        border.color: ApplicationTheme.selectedItem
                        border.width: root.selectedItems[id] ? 3 : 0
                        color: "transparent"
                        Drag.active: itemMouseArea.drag.active
                        Drag.hotSpot.x: width / 2
                        Drag.hotSpot.y: height / 2

                        MouseArea {
                            id: itemMouseArea
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton | Qt.RightButton
                            drag.target: selectMode.checked ? parent : undefined
                            drag.onActiveChanged: {
                                if (itemMouseArea.drag.active) {
                                    root.dragRelease = id;
                                    itemContainer.parent = listViewReleases;
                                    itemContainer.opacity = .7;
                                } else {
                                    if (root.dragRelease > -1 && root.dropRelease === -1) refreshReleases();
                                }
                            }
                            onClicked: {
                                if(mouse.button & Qt.RightButton) {
                                    selectMode.checked = !selectMode.checked;
                                } else {
                                    if (root.selectedItems[id]) {
                                        delete root.selectedItems[id];
                                    } else {
                                        root.selectedItems[id] = true;
                                    }
                                    root.selectedItems = root.selectedItems;
                                }
                            }
                        }
                        Grid {
                            anchors.topMargin: 10
                            columnSpacing: 3
                            rowSpacing: 3
                            columns: 1
                            rows: 2
                            bottomPadding: 4
                            leftPadding: 4
                            topPadding: 4
                            rightPadding: 4

                            Rectangle {
                                width: 260
                                height: 236
                                color: "transparent"

                                Rectangle {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    width: 152
                                    height: 232
                                    border.color: "#adadad"
                                    border.width: 1
                                    radius: 12

                                    Image {
                                        anchors.centerIn: parent
                                        source: localStorage.getReleasePosterPath(id, poster)
                                        sourceSize: Qt.size(150, 230)
                                        fillMode: Image.PreserveAspectCrop
                                        width: 150
                                        height: 230
                                        layer.enabled: true
                                        layer.effect: OpacityMask {
                                            maskSource: mask
                                        }
                                    }
                                }
                            }

                            Rectangle {
                                width: 260
                                height: 38
                                color: "transparent"

                                AccentText {
                                    fontPointSize: 10
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    width: 260
                                    leftPadding: 8
                                    topPadding: 6
                                    wrapMode: Text.WordWrap
                                    maximumLineCount: 2
                                    text: title
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    onNavigateTo: {
        refreshReleases();
    }

    function refreshReleases() {
        releasesModel.clear();
        var releases = JSON.parse(localStorage.getCinemahallReleases());
        for (const release of releases) releasesModel.append(release);
    }

}
