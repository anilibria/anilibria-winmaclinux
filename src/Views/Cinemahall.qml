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
    signal watchCinemahall()

    RowLayout {
        id: panelContainer
        anchors.fill: parent
        spacing: 0
        Rectangle {
            color: ApplicationTheme.pageVerticalPanel
            width: 40
            Layout.fillHeight: true
            Column {
                LeftPanelIconButton {
                    tooltipMessage: "Открыть меню приложения"
                    iconPath: "../Assets/Icons/menu.svg"
                    iconWidth: 29
                    iconHeight: 29
                    onButtonPressed: {
                        drawer.open();
                    }
                }
                LeftPanelIconButton {
                    iconPath: "../Assets/Icons/popcorn.svg"
                    tooltipMessage: "Управление кинозалом"
                    onButtonPressed: {
                        cinemahallMenuPanel.open();
                    }

                    CommonMenu {
                        id: cinemahallMenuPanel
                        y: parent.height
                        width: 300

                        CommonMenuItem {
                            text: "Удалить выбранные релизы"
                            enabled: Object.keys(root.selectedItems).length
                            onPressed: {
                                localStorage.deleteReleasesFromCinemahall(Object.keys(root.selectedItems));
                                root.selectedItems = {};
                                cinemahallMenuPanel.close();

                                refreshReleases();
                            }
                        }
                        CommonMenuItem {
                            text: "Удалить все релизы"
                            onPressed: {
                                localStorage.deleteAllReleasesFromCinemahall()
                                cinemahallMenuPanel.close();

                                refreshReleases();
                            }
                        }
                    }
                }
                LeftPanelIconButton {
                    iconPath: "../Assets/Icons/cinemaplay.svg"
                    tooltipMessage: "Смотреть кинозал в видеоплеере"
                    iconWidth: 29
                    iconHeight: 29
                    onButtonPressed: {
                        watchCinemahall();
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

            Rectangle {
                color: "transparent"
                visible: releasesModel.count === 0
                Layout.fillHeight: true
                Layout.fillWidth: true

                LinkedText {
                    anchors.centerIn: parent
                    fontPointSize: 14
                    horizontalAlignment: Text.AlignHCenter
                    text: `Ваш кинозал пуст.<br />Релизы добавляются в кинозал на странице Каталог релизов.<br />Подробная справка <a href="https://github.com/anilibria/anilibria-winmaclinux/blob/master/cinemahall.md">о том как пользоваться кинозалом</a>.`
                }
            }

            ListView {
                id: listViewReleases
                visible: releasesModel.count > 0
                Layout.fillHeight: true
                Layout.fillWidth: true
                orientation: ListView.Horizontal
                model: releasesModel
                clip: true
                ScrollBar.horizontal: ScrollBar {
                    active: true
                }
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
                        color: root.dragRelease !== root.dropRelease && root.dropRelease === id ? "#82000000" : "transparent"
                        Drag.active: itemMouseArea.drag.active
                        Drag.hotSpot.x: width / 2
                        Drag.hotSpot.y: height / 2
                        property bool showTextHeader: true

                        MouseArea {
                            id: itemMouseArea
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton | Qt.RightButton
                            drag.target: selectMode.checked ? parent : undefined
                            drag.onActiveChanged: {
                                if (itemMouseArea.drag.active) {
                                    root.dragRelease = id;
                                    root.dropRelease = -1;
                                    itemContainer.parent = listViewReleases;
                                    itemContainer.opacity = .7;
                                    itemContainer.showTextHeader = false;
                                } else {
                                    if (root.dragRelease > -1 && root.dropRelease > -1) {
                                        localStorage.reorderReleaseInCinemahall(root.dragRelease, root.dropRelease);
                                        root.dragRelease = -1;
                                        root.dropRelease = -1;
                                    }
                                    refreshReleases();
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
                                    visible: itemContainer.showTextHeader
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

                    DropArea {
                        anchors.fill: parent
                        onEntered: {
                            root.dropRelease = id;
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
