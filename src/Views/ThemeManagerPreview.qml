import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0
import "../Controls"

ApplicationWindow {
    id: previewWindow
    minimumWidth: 500
    minimumHeight: 400
    width: 800
    height: 650
    flags: Qt.Dialog | Qt.WindowTitleHint | Qt.WindowCloseButtonHint
    modality: Qt.ApplicationModal
    title: "Предпросмотр темы"
    Material.accent: applicationThemeViewModel.previewItems.materialAccent
    //Material.foreground: "red"
    //Material.background: "red"
    //Material.hintTextColor
    Material.theme: applicationThemeViewModel.fieldList.basedOnTheme === 'Темная' ? Material.Dark : Material.Light
    background: Rectangle {
        color: applicationThemeViewModel.previewItems.pageBackground
    }

    header: Rectangle {
        id: toolBar
        width: previewWindow.width
        height: 35
        color: applicationThemeViewModel.previewItems.notificationCenterBackground

        Item {
            id: titleArea
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height

            Text {
                id: taskbarTitle
                anchors.centerIn: parent
                font.pointSize: 12 + windowSettings.dpiSeparation
                color: applicationThemeViewModel.previewItems.headerTextColor
                text: "Текст заголовка"
            }
        }
    }

    footer: Rectangle {
        width: previewWindow.width
        height: 16
        color: applicationThemeViewModel.previewItems.notificationCenterBackground

        Text {
            anchors.centerIn: parent
            font.pointSize: 10 + windowSettings.dpiSeparation
            color: applicationThemeViewModel.previewItems.plainTextColor
            text: "Текст панели уведомлений"
        }

        Item {
            anchors.right: parent.right
            width: 16
            height: 16

            MouseArea {
                anchors.fill: parent
                onPressed: {
                    notificationOverlay.visible = !notificationOverlay.visible;
                }
            }

            Rectangle {
                anchors.fill: parent
                color: "transparent"

                Image {
                    id: notificationIconImage
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: assetsLocation.iconsPath + "notification.svg"
                    width: 14
                    height: 14
                    mipmap: true
                }

                ColorOverlay {
                    anchors.fill: notificationIconImage
                    source: notificationIconImage
                    color: "#9e2323"
                }
            }
        }
    }

    RowLayout {
        id: panelContainer
        anchors.fill: parent
        spacing: 0
        Rectangle {
            color: applicationThemeViewModel.previewItems.pageVerticalPanel
            width: 40
            Layout.fillHeight: true

            Item {
                id: iconButton
                width: 40
                height: 45
                property bool hovered: false

                MouseArea {
                    id: mouseArea
                    hoverEnabled: true
                    anchors.fill: parent
                    onEntered: {
                        imageRectangle.color = applicationThemeViewModel.previewItems.filterIconButtonHoverColor;
                        iconButton.hovered = true;
                    }
                    onExited: {
                        imageRectangle.color = "transparent";
                        iconButton.hovered = false;
                    }
                    onPressed: {
                        drawer.open();
                    }
                }

                Rectangle {
                    id: imageRectangle
                    anchors.fill: parent
                    color: "transparent"

                    Image {
                        id: iconImage
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: applicationThemeViewModel.previewItems.iconMainMenu
                        width: 28
                        height: 28
                        mipmap: true
                    }
                }
                ToolTip.delay: 1000
                ToolTip.visible: hovered
                ToolTip.text: "Текст в тултипе"
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
                color: applicationThemeViewModel.previewItems.pageUpperPanel

                Text {
                    anchors.left: parent.left
                    anchors.leftMargin: 4
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Обычный текст в первом заголовке страницы"
                    font.pointSize: 11 + windowSettings.dpiSeparation
                    color: applicationThemeViewModel.previewItems.plainTextColor
                    linkColor: applicationThemeViewModel.previewItems.linkTextColor
                }
            }

            Rectangle {
                color: "transparent"
                Layout.fillHeight: true
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    Item {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 45

                        Text {
                            anchors.left: parent.left
                            anchors.leftMargin: 4
                            anchors.verticalCenter: parent.verticalCenter
                            text: "Обычный текст во втором заголовке страницы"
                            font.pointSize: 11 + windowSettings.dpiSeparation
                            color: applicationThemeViewModel.previewItems.plainTextColor
                            linkColor: applicationThemeViewModel.previewItems.linkTextColor
                        }
                    }
                    Item {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        Rectangle {
                            id: plainReleaseItem
                            anchors.left: parent.left
                            anchors.leftMargin: 8
                            width: 280
                            height: 280
                            radius: 10
                            color: applicationThemeViewModel.previewItems.panelBackground
                            layer.enabled: true
                            layer.effect: DropShadow {
                                transparentBorder: true
                                horizontalOffset: 2
                                verticalOffset: 2
                                radius: 1
                                samples: 3
                                color: applicationThemeViewModel.previewItems.panelBackgroundShadow
                            }

                            Rectangle {
                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.leftMargin: 4
                                anchors.topMargin: 4
                                width: 182
                                height: 272
                                border.color: applicationThemeViewModel.previewItems.posterBorder
                                border.width: 1
                                radius: 12

                                Rectangle {
                                    id: mask
                                    width: 180
                                    height: 260
                                    radius: 10
                                    visible: false
                                }

                                Image {
                                    anchors.centerIn: parent
                                    source: "https://kawai.shikimori.one/system/animes/original/51306.jpg"
                                    sourceSize: Qt.size(350, 500)
                                    fillMode: Image.PreserveAspectCrop
                                    width: 180
                                    height: 270
                                    layer.enabled: true
                                    layer.effect: OpacityMask {
                                        maskSource: mask
                                    }
                                }
                            }
                        }
                        Rectangle {
                            id: selectedReleaseItem
                            anchors.left: plainReleaseItem.right
                            anchors.leftMargin: 10
                            width: 280
                            height: 280
                            radius: 10
                            border.color: applicationThemeViewModel.previewItems.selectedItem
                            border.width: 3
                            color: applicationThemeViewModel.previewItems.panelBackground
                            layer.enabled: true
                            layer.effect: DropShadow {
                                transparentBorder: true
                                horizontalOffset: 2
                                verticalOffset: 2
                                radius: 1
                                samples: 3
                                color: applicationThemeViewModel.previewItems.panelBackgroundShadow
                            }

                            Text {
                                id: headerText
                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.leftMargin: 10
                                anchors.topMargin: 10
                                font.pointSize: 11 + windowSettings.dpiSeparation
                                color: applicationThemeViewModel.previewItems.headerTextColor
                                text: "Текст заголовка"
                            }
                            Text {
                                anchors.top: headerText.bottom
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                text: "Обычный текст <br><b>Жирный обычный текст</b><br><a href='https://anilibria.tv'>Обычный текст со ссылкой</a>"
                                font.pointSize: 10 + windowSettings.dpiSeparation
                                color: applicationThemeViewModel.previewItems.plainTextColor
                                linkColor: applicationThemeViewModel.previewItems.linkTextColor
                            }
                        }

                        Item {
                            id: roundedButtonActive
                            anchors.left: parent.left
                            anchors.top: plainReleaseItem.bottom
                            anchors.topMargin: 10
                            width: roundedButtonActiveTitle.width + 34
                            height: 34

                            Rectangle {
                                anchors.fill: parent
                                anchors.leftMargin: 8
                                anchors.rightMargin: 8
                                anchors.topMargin: 4
                                anchors.bottomMargin: 4
                                color: applicationThemeViewModel.previewItems.roundedButtonBackground
                                border.color: "transparent"
                                border.width: 1
                                radius: 18

                                Text {
                                    id: roundedButtonActiveTitle
                                    font.pointSize: 10 + windowSettings.dpiSeparation
                                    anchors.centerIn: parent
                                    text: "Активная кнопка"
                                    color: applicationThemeViewModel.previewItems.roundedButtonForeground
                                }
                            }
                        }

                        Item {
                            id: roundedButtonHovered
                            anchors.left: roundedButtonActive.right
                            anchors.top: plainReleaseItem.bottom
                            anchors.topMargin: 10
                            width: roundedButtonHoveredTitle.width + 34
                            height: 34

                            Rectangle {
                                anchors.fill: parent
                                anchors.leftMargin: 8
                                anchors.rightMargin: 8
                                anchors.topMargin: 4
                                anchors.bottomMargin: 4
                                color: applicationThemeViewModel.previewItems.roundedButtonHovered
                                border.color: "transparent"
                                border.width: 1
                                radius: 18

                                Text {
                                    id: roundedButtonHoveredTitle
                                    font.pointSize: 10 + windowSettings.dpiSeparation
                                    anchors.centerIn: parent
                                    text: "Активная кнопка с мышкой поверх"
                                    color: applicationThemeViewModel.previewItems.roundedButtonForeground
                                }
                            }
                        }

                        Item {
                            id: roundedButtonDisabled
                            anchors.left: roundedButtonHovered.right
                            anchors.top: plainReleaseItem.bottom
                            anchors.topMargin: 10
                            width: roundedButtonDisabledTitle.width + 34
                            height: 34

                            Rectangle {
                                anchors.fill: parent
                                anchors.leftMargin: 8
                                anchors.rightMargin: 8
                                anchors.topMargin: 4
                                anchors.bottomMargin: 4
                                color: applicationThemeViewModel.previewItems.roundedButtonBackgroundDisabled
                                border.color: "transparent"
                                border.width: 1
                                radius: 18

                                Text {
                                    id: roundedButtonDisabledTitle
                                    font.pointSize: 10 + windowSettings.dpiSeparation
                                    anchors.centerIn: parent
                                    text: "Не активная кнопка"
                                    color: applicationThemeViewModel.previewItems.roundedButtonDisableForeground
                                }
                            }
                        }

                        Item {
                            id: activePlaylistItem
                            anchors.top: roundedButtonActive.bottom
                            anchors.topMargin: 10
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            height: 40

                            Rectangle {
                                height: parent.height
                                width: 200
                                color: applicationThemeViewModel.previewItems.playlistBackground

                                Text {
                                    color: applicationThemeViewModel.previewItems.playlistText
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    anchors.leftMargin: 10
                                    text: "Активная серия"
                                }

                                Row {
                                    width: 36
                                    spacing: 0
                                    anchors.right: parent.right
                                    anchors.rightMargin: 18

                                    Item {
                                        id: seenButton
                                        width: 36
                                        height: 36

                                        Rectangle {
                                            anchors.fill: parent
                                            color: "transparent"

                                            Image {
                                                id: activeIconImage
                                                anchors.verticalCenter: parent.verticalCenter
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                source: assetsLocation.iconsPath + "seenmarkselected.svg"
                                                width: 22
                                                height: 22
                                                mipmap: true
                                            }

                                            ColorOverlay {
                                                id: colorOverlay
                                                anchors.fill: activeIconImage
                                                source: activeIconImage
                                                color: applicationThemeViewModel.previewItems.filterIconButtonGreenColor
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        Item {
                            id: playlistControls
                            anchors.top: activePlaylistItem.bottom
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            height: 40

                            Rectangle {
                                height: parent.height
                                width: 200
                                color: applicationThemeViewModel.previewItems.playlistSelectedBackground

                                Text {
                                    color: applicationThemeViewModel.previewItems.playlistSelectedText
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    anchors.leftMargin: 10
                                    text: "Текущая серия"
                                }

                                Row {
                                    width: 36
                                    spacing: 0
                                    anchors.right: parent.right
                                    anchors.rightMargin: 18

                                    Item {
                                        id: notSeenButton
                                        width: 36
                                        height: 36

                                        Rectangle {
                                            anchors.fill: parent
                                            color: "transparent"

                                            Image {
                                                id: selectedIconImage
                                                anchors.verticalCenter: parent.verticalCenter
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                source: assetsLocation.iconsPath + "seenmark.svg"
                                                width: 22
                                                height: 22
                                                mipmap: true
                                            }

                                            ColorOverlay {
                                                id: selectedColorOverlay
                                                anchors.fill: selectedIconImage
                                                source: selectedIconImage
                                                color: applicationThemeViewModel.previewItems.filterIconButtonColor
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        Item {
                            id: playerControls
                            width: 400
                            height: 50
                            anchors.left: parent.left
                            anchors.leftMargin: 220
                            anchors.top: activePlaylistItem.top

                            Rectangle {
                                anchors.fill: parent
                                color: applicationThemeViewModel.previewItems.playerControlBackground
                            }

                            Row {
                                height: 20
                                anchors.centerIn: parent
                                spacing: 10

                                Rectangle {
                                    id: toggleButtonHoveredToggle
                                    height: 20
                                    width: 100
                                    color: applicationThemeViewModel.previewItems.toggleButtonHovered

                                    Text {
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        anchors.verticalCenter: parent.verticalCenter
                                        color: applicationThemeViewModel.previewItems.plainTextColor
                                        text: "поверх"
                                    }
                                }
                                Rectangle {
                                    id: toggleButtonCheckedToggle
                                    height: 20
                                    width: 100
                                    color: applicationThemeViewModel.previewItems.toggleButtonChecked

                                    Text {
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        anchors.verticalCenter: parent.verticalCenter
                                        color: applicationThemeViewModel.previewItems.plainTextColor
                                        text: "выбрано"
                                    }
                                }
                                Rectangle {
                                    id: toggleButtonNotCheckedToggle
                                    height: 20
                                    width: 100
                                    color: applicationThemeViewModel.previewItems.toggleButtonNotChecked

                                    Text {
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        anchors.verticalCenter: parent.verticalCenter
                                        color: applicationThemeViewModel.previewItems.plainTextColor
                                        text: "не выбрано"
                                    }
                                }
                            }
                        }
                        Item {
                            width: 400
                            height: 50
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            anchors.top: playlistControls.bottom

                            Switch {
                                id: previewSwitch
                            }

                            TextField {
                                anchors.left: previewSwitch.right
                                anchors.leftMargin: 10
                                width: 300
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: notificationOverlay
        width: 240
        height: window.height
        anchors.right: parent.right
        anchors.topMargin: -2
        anchors.rightMargin: -1
        color: applicationThemeViewModel.previewItems.notificationCenterPanelBackground
        border.color: applicationThemeViewModel.previewItems.selectedItem
        border.width: 1
        visible: false

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onPressed: {
            }
        }

        Rectangle {
            width: 230
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            height: 70
            radius: 8
            border.color: applicationThemeViewModel.previewItems.selectedItem
            border.width: 2
            color: applicationThemeViewModel.previewItems.notificationCenterItemBackground

            Text {
                padding: 10
                maximumLineCount: 3
                font.pointSize: 8
                width: parent.width
                wrapMode: Text.WordWrap
                elide: Text.ElideRight
                color: applicationThemeViewModel.previewItems.plainTextColor
                anchors.verticalCenter: parent.verticalCenter
                text: "Текст сообщения"
            }
            Image {
                height: 14
                width: 14
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.rightMargin: 4
                anchors.topMargin: 4
                source: assetsLocation.iconsPath + "coloredclosewindow.svg"
                mipmap: true
            }
        }
    }

    Drawer {
        id: drawer
        width: 300
        dragMargin: 0
        height: window.height
        background: Canvas {
            id: canvas
            anchors.fill: parent
            onPaint: {
                const context = getContext("2d")

                var gradient = context.createLinearGradient(canvas.width / 2, 0, canvas.width / 2, canvas.height);
                gradient.addColorStop(0, applicationThemeViewModel.previewItems.drawerGradiendStep0);
                gradient.addColorStop(0.3, applicationThemeViewModel.previewItems.drawerGradiendStep1);
                gradient.addColorStop(0.5, applicationThemeViewModel.previewItems.drawerGradiendStep2);
                gradient.addColorStop(0.7, applicationThemeViewModel.previewItems.drawerGradiendStep3);
                gradient.addColorStop(1, applicationThemeViewModel.previewItems.drawerGradiendStep4);
                context.fillStyle = gradient;
                context.fillRect(0, 0, canvas.width, canvas.height);
            }
        }
    }

    onClosing: {
        drawer.close();
        notificationOverlay.visible = false;
    }
}
