/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2020 Roman Vladimirov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import "../Controls"

Page {
    id: youtubePage
    anchors.fill: parent
    property int page: 0
    property var allItemsPage

    signal navigateFrom()
    signal navigateTo()

    background: Rectangle {
        color: applicationThemeViewModel.pageBackground
    }

    Loader {
        anchors.fill: parent
        sourceComponent: youtubePage.visible ? youtubePageContent : null
    }

    Component {
        id: youtubePageContent

        Item {
            anchors.fill: parent

            Rectangle {
                id: mask
                width: 472
                height: 240
                radius: 10
                visible: false
            }

            RowLayout {
                id: panelContainer
                anchors.fill: parent
                spacing: 0
                Rectangle {
                    color: applicationThemeViewModel.pageVerticalPanel
                    width: 40
                    Layout.fillHeight: true
                    Column {
                        LeftPanelIconButton {
                            overlayVisible: false
                            iconWidth: 28
                            iconHeight: 28
                            iconPath: assetsLocation.iconsPath + "coloreddrawer.svg"
                            tooltipMessage: "Открыть меню приложения"
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
                        color: applicationThemeViewModel.pageUpperPanel
                    }

                    GridView {
                        id: scrollView
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        cellWidth: parent.width / Math.floor(parent.width / 480)
                        cellHeight: 298
                        model: youtubeViewModel
                        clip: true
                        ScrollBar.vertical: ScrollBar {
                            active: true
                        }
                        delegate: Rectangle {
                            color: "transparent"
                            width: scrollView.cellWidth
                            height: scrollView.cellHeight

                            Rectangle {
                                anchors.centerIn: parent
                                width: 480
                                height: 290
                                radius: 10
                                color: applicationThemeViewModel.panelBackground
                                layer.enabled: true
                                layer.effect: DropShadow {
                                    transparentBorder: true
                                    horizontalOffset: 2
                                    verticalOffset: 2
                                    radius: 1
                                    samples: 3
                                    color: applicationThemeViewModel.panelBackgroundShadow
                                }
                                MouseArea {
                                    width: 480
                                    height: 290
                                    onClicked: {
                                        Qt.openUrlExternally(videoUrl);
                                    }
                                }
                                Grid {
                                    columnSpacing: 3
                                    rows: 2
                                    bottomPadding: 4
                                    leftPadding: 4
                                    topPadding: 4
                                    rightPadding: 4

                                    Image {
                                        source: image
                                        fillMode: Image.PreserveAspectCrop
                                        width: 472
                                        height: 240
                                        layer.enabled: true
                                        layer.effect: OpacityMask {
                                            maskSource: mask
                                        }
                                    }

                                    PlainText {
                                        Layout.row: 1
                                        width: 460
                                        textFormat: Text.RichText
                                        fontPointSize: 10
                                        leftPadding: 8
                                        topPadding: 4
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

            Rectangle {
                color: "transparent"
                width: 190
                height: 50
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.bottom: parent.bottom

                LeftPanelIconButton {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.topMargin: 8
                    iconColor: applicationThemeViewModel.filterIconButtonColor
                    hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                    tooltipMessage: "Вернуться в начало списка релизов"
                    visible: scrollView.contentY > 100
                    iconPath: "../Assets/Icons/arrowup.svg"
                    iconWidth: 24
                    iconHeight: 24
                    onButtonPressed: {
                        scrollView.contentY = 0;
                    }

                }
            }
        }
    }
}

