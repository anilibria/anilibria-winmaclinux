import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../Controls"

Page {
    id: root
    anchors.fill: parent
    background: Rectangle {
        color: applicationThemeViewModel.pageBackground
    }

    signal watchCinemahall()

    Loader {
        anchors.fill: parent
        sourceComponent: root.visible ? cinemahallPageContent : null
    }

    Component {
        id: cinemahallPageContent

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
                        id: upperPanelContainer
                        Layout.fillWidth: true
                        Layout.preferredHeight: 45
                        height: 45
                        color: applicationThemeViewModel.pageUpperPanel

                        CommonSwitch {
                            id: selectMode
                            anchors.left: parent.left
                            onCheckedChanged: {
                                releasesViewModel.cinemahall.deselectItems();
                            }
                            tooltipMessage: "Данный переключатель влияет на поведение при клике ЛКМ на релизах в списке\nРежим выбора выбор позволяет выбрать несколько релизов и выполнять действия(например удаление)\nРежим изменения порядка позволяет изменять порядок релизов в списке путем перетаскивания\nЧтобы переключать его можно использовать клик ПКМ в области списка релизов"
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
                            anchors.verticalCenter: parent.verticalCenter
                            width: 210
                            buttonEnabled: releasesViewModel.cinemahall.hasItems
                            textSize: 10
                            text: "Удалить просмотренное"
                            allowedRightClick: true
                            onClicked: {
                                releasesViewModel.cinemahall.restoreScrollPosition = listViewReleases.contentX;
                                releasesViewModel.cinemahall.deletedSeenReleases();
                            }
                            onRightClicked: {
                                deleteSeensPopup.open();
                            }

                            DefaultPopup {
                                id: deleteSeensPopup
                                width: 390
                                height: 130
                                modal: true
                                focus: true
                                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                                Column {
                                    width: parent.width
                                    spacing: 10

                                    PlainText {
                                        width: deleteSeensPopup.width - 20
                                        fontPointSize: 10
                                        text: "Другие варианты удаления"
                                    }

                                    RoundedActionButton {
                                        anchors.right: parent.right
                                        anchors.left: parent.left
                                        buttonEnabled: releasesViewModel.cinemahall.hasItems
                                        textSize: 10
                                        text: "Удалить из кинозала и избранного"
                                        onClicked: {
                                            releasesViewModel.cinemahall.restoreScrollPosition = listViewReleases.contentX;
                                            releasesViewModel.cinemahall.deletedSeenAndFavoritesReleases();
                                            deleteSeensPopup.close();
                                        }
                                    }

                                    RoundedActionButton {
                                        anchors.right: parent.right
                                        anchors.left: parent.left
                                        buttonEnabled: releasesViewModel.cinemahall.hasItems
                                        textSize: 10
                                        text: "Удалить только из избранного"
                                        onClicked: {
                                            releasesViewModel.cinemahall.deletedSeenOnlyFromFavorites();
                                            deleteSeensPopup.close();
                                        }
                                    }
                                }
                            }

                        }

                        RoundedActionButton {
                            id: deleteSelectedButton
                            anchors.right: deleteAllReleasesButton.left
                            anchors.verticalCenter: parent.verticalCenter
                            width: 210
                            buttonEnabled: releasesViewModel.cinemahall.hasSelectedItems
                            textSize: 10
                            text: "Удалить выбранное"
                            onClicked: {
                                releasesViewModel.cinemahall.restoreScrollPosition = listViewReleases.contentX;
                                releasesViewModel.cinemahall.deleteSelectedReleases();
                            }
                        }

                        RoundedActionButton {
                            id: deleteAllReleasesButton
                            anchors.verticalCenter: parent.verticalCenter
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
                            text: `Ваш кинозал пуст.<br />Релизы добавляются в кинозал на странице Каталог релизов.<br />
                            Подробная текстовая справка <a href="https://github.com/anilibria/anilibria-winmaclinux/blob/master/cinemahall.md">о том как пользоваться кинозалом</a>.
                            <br />Также есть <a href="https://t.me/desktopclientanilibria/275">видеоинструкция</a>.`
                        }
                    }

                    ListView {
                        id: listViewReleases
                        visible: releasesViewModel.cinemahall.hasItems
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        orientation: ListView.Horizontal
                        boundsBehavior: Flickable.StopAtBounds
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
                                border.color: applicationThemeViewModel.selectedItem
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
                                            releasesViewModel.cinemahall.restoreScrollPosition = listViewReleases.contentX;
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

                                        PlainText {
                                            fontPointSize: 10
                                            anchors.centerIn: parent
                                            text: seenSeries
                                        }

                                        IconButton {
                                            anchors.right: parent.right
                                            anchors.rightMargin: 26
                                            width: 30
                                            height: 30
                                            iconWidth: 20
                                            iconHeight: 20
                                            hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                                            iconPath: applicationThemeViewModel.currentItems.iconContextMenu
                                            onButtonPressed: {
                                                releasesViewModel.cinemahall.openedItemIndex = releaseNumber - 1;
                                                releasesViewModel.cinemahall.dragRelease = id;
                                                cinemahallItemContextMenu.open();
                                            }

                                            CommonMenuModel {
                                                id: cinemahallItemContextMenu
                                                width: 300
                                                menuModel: releasesViewModel.cinemahall.itemMenuList
                                                onBeforeAction: {
                                                    releasesViewModel.cinemahall.restoreScrollPosition = listViewReleases.contentX;
                                                }
                                            }
                                        }
                                    }

                                    Rectangle {
                                        width: elements.width
                                        height: 236
                                        color: "transparent"

                                        CorneredImage {
                                            id: posterContainer
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            width: 152
                                            height: 232
                                            posterSource: localStorage.getReleasePosterPath(id, poster)
                                            emptyBorderBackground: applicationThemeViewModel.currentItems.pageBackground
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
                                            color: applicationThemeViewModel.colorPageIndexText
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
                    color: applicationThemeViewModel.roundedButtonBackground
                    anchors.fill: parent
                    radius: 10
                }

                PlainText {
                    anchors.fill: parent
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                    fontPointSize: 12
                    color: applicationThemeViewModel.roundedButtonForeground
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
                    source: applicationThemeViewModel.iconMainMenuCinemahall
                }

                Image {
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    anchors.verticalCenter: parent.verticalCenter
                    mipmap: true
                    opacity: .7
                    width: 32
                    height: 30
                    source: applicationThemeViewModel.iconMainMenuCinemahall
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onPressed: {
                        watchCinemahall();
                    }
                    onEntered: {
                        backgroundRectangle.color = applicationThemeViewModel.roundedButtonHovered;
                    }
                    onExited: {
                        backgroundRectangle.color = applicationThemeViewModel.roundedButtonBackground;
                    }
                }
            }

            Connections {
                target: releasesViewModel.cinemahall
                function onVisibleSetupMovedNumber() {
                    setupNumberToMove.open();
                }
                function onNeedRestoreScrollPosition() {
                    listViewReleases.contentX = releasesViewModel.cinemahall.restoreScrollPosition;
                }
            }

            MessageModal {
                id: setupNumberToMove
                header: "Укажите индекс для перемещения"
                messageHeight: 0
                content: Column {
                    id: numbersColumnnContainer
                    width: setupNumberToMove.width - 20

                    CommonTextField {
                        id: movedIndexTextField
                        width: numbersColumnnContainer.width
                        placeholderText: releasesViewModel.cinemahall.movedPositionPlaceholder
                        text: ""
                        onTextChanged: {
                            let number = parseInt(text);
                            if (isNaN(number)) number = 0;
                            releasesViewModel.cinemahall.movedPositionIndex = number;
                        }
                    }

                    Item {
                        width: numbersColumnnContainer.width
                        height: 40

                        RoundedActionButton {
                            text: "Перейти"
                            buttonEnabled: releasesViewModel.cinemahall.validMovedPosition
                            anchors.right: cancelButton.left
                            width: 100
                            onClicked: {
                                releasesViewModel.cinemahall.moveToTypedNumber();
                                releasesViewModel.cinemahall.movedPositionIndex = 0;
                                movedIndexTextField.text = "";
                                setupNumberToMove.close();
                            }
                        }
                        RoundedActionButton {
                            id: cancelButton
                            anchors.right: parent.right
                            text: "Отмена"
                            width: 100
                            onClicked: {
                                releasesViewModel.cinemahall.movedPositionIndex = 0;
                                movedIndexTextField.text = "";
                                setupNumberToMove.close();
                            }
                        }
                    }

                }
            }
        }
    }
}
