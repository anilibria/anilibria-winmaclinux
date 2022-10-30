import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0
import "../Controls"

ApplicationWindow {
    id: previewWindow
    minimumWidth: 500
    minimumHeight: 300
    width: 800
    height: 600
    flags: Qt.Dialog | Qt.WindowTitleHint | Qt.WindowCloseButtonHint
    modality: Qt.ApplicationModal
    title: "Предпросмотр темы"
    Material.accent: Material.Red
    Material.theme: applicationThemeViewModel.fieldList.basedOnTheme === 'Темная' ? Material.Dark : Material.Light
    background: Rectangle {
        color: applicationThemeViewModel.previewItems.pageBackground
    }

    RowLayout {
        id: panelContainer
        anchors.fill: parent
        spacing: 0
        Rectangle {
            color: applicationThemeViewModel.previewItems.pageVerticalPanel
            width: 40
            Layout.fillHeight: true

            IconButton {
                height: 45
                width: 40
                overlayVisible: false
                iconPath: assetsLocation.iconsPath + "coloreddrawer.svg"
                iconWidth: 28
                iconHeight: 28
                onButtonPressed: {
                    drawer.open();
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
                                border.color: "#adadad"
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
                    }
                }
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
    }
}
