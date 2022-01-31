import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.3
import "../Theme"
import "../Controls"

Page {
    id: root
    anchors.fill: parent
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
                    iconPath: assetsLocation.iconsPath + "menu.svg"
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
                id: upperPanelContainer
                Layout.fillWidth: true
                Layout.preferredHeight: 45
                height: 45
                color: ApplicationTheme.pageUpperPanel

                Switch {
                    id: selectMode
                    anchors.left: parent.left
                    onCheckedChanged: {
                        releasesViewModel.cinemahall.deselectItems();
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

                RoundedActionButton {
                    anchors.right: deleteSelectedButton.left
                    width: 210
                    buttonEnabled: releasesViewModel.cinemahall.hasItems
                    textSize: 10
                    text: "Удалить просмотренное"
                    onClicked: {
                        releasesViewModel.cinemahall.deletedSeenReleases();
                    }
                }

                RoundedActionButton {
                    id: deleteSelectedButton
                    anchors.right: deleteAllReleasesButton.left
                    width: 210
                    buttonEnabled: releasesViewModel.cinemahall.hasSelectedItems
                    textSize: 10
                    text: "Удалить выбранное"
                    onClicked: {
                        releasesViewModel.cinemahall.deleteSelectedReleases();
                    }
                }

                RoundedActionButton {
                    id: deleteAllReleasesButton
                    anchors.right: parent.right
                    width: 210
                    buttonEnabled: releasesViewModel.cinemahall.hasItems
                    textSize: 10
                    text: "Удалить все"
                    onClicked: {
                        releasesViewModel.cinemahall.deleteAllReleases();
                    }
                }
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
                visible: !releasesViewModel.cinemahall.hasItems
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
                visible: releasesViewModel.cinemahall.hasItems
                Layout.fillHeight: true
                Layout.fillWidth: true
                orientation: ListView.Horizontal
                model: releasesViewModel.cinemahall
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
                        border.width: isSelected ? 3 : 0
                        color: releasesViewModel.cinemahall.dragRelease !== releasesViewModel.cinemahall.dropRelease && releasesViewModel.cinemahall.dropRelease === id ? "#82000000" : "transparent"
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
                                    releasesViewModel.cinemahall.dragRelease = id;
                                    releasesViewModel.cinemahall.dropRelease = -1;
                                    itemContainer.parent = listViewReleases;
                                    itemContainer.opacity = .7;
                                    itemContainer.showTextHeader = false;
                                } else {
                                    releasesViewModel.cinemahall.reorderRelease();
                                }
                            }
                            onClicked: {
                                if(mouse.button & Qt.RightButton) {
                                    selectMode.checked = !selectMode.checked;
                                    return;
                                }

                                releasesViewModel.cinemahall.selectItem(id);
                            }
                        }
                        Column {
                            id: elements
                            width: 260
                            anchors.topMargin: 10
                            spacing: 3
                            bottomPadding: 4
                            leftPadding: 4
                            topPadding: 4
                            rightPadding: 4

                            Item {
                                width: elements.width
                                height: 30

                                IconButton {
                                    anchors.right: parent.right
                                    anchors.rightMargin: 20
                                    width: 30
                                    height: 30
                                    iconWidth: 20
                                    iconHeight: 20
                                    hoverColor: ApplicationTheme.filterIconButtonHoverColor
                                    overlayVisible: false
                                    iconPath: assetsLocation.iconsPath + "contextmenu.svg"
                                    onButtonPressed: {
                                        releasesViewModel.cinemahall.openedItemIndex = releaseNumber - 1;
                                        releasesViewModel.cinemahall.dragRelease = id;
                                        cinemahallItemContextMenu.open();
                                    }

                                    CommonMenuModel {
                                        id: cinemahallItemContextMenu
                                        width: 300
                                        menuModel: releasesViewModel.cinemahall.itemMenuList
                                    }
                                }
                            }

                            Rectangle {
                                width: elements.width
                                height: 236
                                color: "transparent"

                                Rectangle {
                                    id: posterContainer
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

                                Image {
                                    id: bookmarkImage
                                    anchors.left: posterContainer.left
                                    anchors.leftMargin: -7
                                    width: 50
                                    height: 50
                                    source: assetsLocation.iconsPath + "numberbookmark.svg"
                                }

                                PlainText {
                                    anchors.left: bookmarkImage.left
                                    anchors.top: bookmarkImage.top
                                    anchors.topMargin: 22
                                    width: 40
                                    horizontalAlignment: Qt.AlignHCenter
                                    text: releaseNumber
                                    fontPointSize: 10
                                    color: "white"
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
                            releasesViewModel.cinemahall.dropRelease = id;
                        }
                    }
                }
            }
        }
    }

    Item {
        anchors.top: parent.top
        anchors.topMargin: 48
        anchors.horizontalCenter: parent.horizontalCenter
        visible: releasesViewModel.cinemahall.hasItems
        width: 260
        height: 40

        Rectangle {
            id: backgroundRectangle
            color: ApplicationTheme.roundedButtonBackground
            anchors.fill: parent
            radius: 10
        }

        PlainText {
            anchors.fill: parent
            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignHCenter
            fontPointSize: 12
            color: ApplicationTheme.roundedButtonForeground
            text: "Смотреть кинозал"
        }

        Image {
            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            mipmap: true
            opacity: .7
            width: 32
            height: 30
            source: assetsLocation.iconsPath + "cinemahallmenu.svg"
        }

        Image {
            anchors.right: parent.right
            anchors.rightMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            mipmap: true
            opacity: .7
            width: 32
            height: 30
            source: assetsLocation.iconsPath + "cinemahallmenu.svg"
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onPressed: {
                watchCinemahall();
            }
            onEntered: {
                backgroundRectangle.color = ApplicationTheme.roundedButtonHovered;
            }
            onExited: {
                backgroundRectangle.color = ApplicationTheme.roundedButtonBackground;
            }
        }
    }

}
