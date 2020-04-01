import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import "../Controls"

Page {
    id: aboutPage
    anchors.fill: parent

    signal navigateFrom()
    signal navigateTo()

    background: Rectangle {
        color: "#D3D3D3"
    }

    RowLayout {
        id: panelContainer
        anchors.fill: parent
        spacing: 0
        Rectangle {
            color: "#9e2323"
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
                color: "#808080"
            }

            Flickable {
                id: scrollview
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignJustify
                clip: true
                contentWidth: parent.width
                contentHeight: itemGrid.height
                ScrollBar.vertical: ScrollBar {
                    active: true
                }

                ColumnLayout {
                    spacing: 2
                    id: itemGrid
                    width: scrollview.width
                    height: 800
                    Layout.alignment: Qt.AlignHCenter

                    Rectangle {
                        color: "transparent"
                        width: 200
                        height: 200
                        Layout.fillWidth: true
                        Layout.preferredHeight: 200

                        Image {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            source: "../Assets/Icons/anilibrialogodefault.svg"
                            mipmap: true
                            width: 200
                            height: 200
                        }
                    }

                    Rectangle {
                        color: "transparent"
                        width: 400
                        height: 40
                        Layout.alignment: Qt.AlignCenter
                        Layout.fillWidth: true
                        Layout.preferredHeight: 40

                        Text {
                            anchors.centerIn: parent
                            font.pixelSize: 24
                            text: "Клиент для сайта AniLibria.TV"
                        }
                    }

                    Rectangle {
                        color: "transparent"
                        width: 400
                        height: 40
                        Layout.alignment: Qt.AlignCenter
                        Layout.fillWidth: true
                        Layout.preferredHeight: 40

                        Text {
                            anchors.centerIn: parent
                            font.pixelSize: 18
                            linkColor: "#b32121"
                            text: "<a href='https://www.anilibria.tv/'>https://www.anilibria.tv/</a>"
                            onLinkActivated: {
                                Qt.openUrlExternally(link);
                            }
                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.NoButton
                                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                            }
                        }
                    }

                    Rectangle {
                        color: "transparent"
                        width: 400
                        height: 40
                        Layout.alignment: Qt.AlignCenter
                        Layout.fillWidth: true
                        Layout.preferredHeight: 40

                        Text {
                            anchors.centerIn: parent
                            font.pixelSize: 16
                            text: "версия " + ApplicationVersion
                        }
                    }

                    Rectangle {
                        color: "transparent"
                        width: 400
                        height: 30
                        Layout.alignment: Qt.AlignCenter
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30

                        Text {
                            anchors.centerIn: parent
                            font.pixelSize: 18
                            color: "#b32121"
                            text: "Спасибо что выбираете нас!"
                        }
                    }

                    Rectangle {
                        color: "transparent"
                        width: 400
                        height: 15
                        Layout.alignment: Qt.AlignCenter
                        Layout.fillWidth: true
                        Layout.preferredHeight: 15

                        Text {
                            anchors.centerIn: parent
                            font.pixelSize: 14
                            wrapMode: Text.WordWrap
                            width: parent.width
                            horizontalAlignment: Text.AlignHCenter
                            linkColor: "#b32121"
                            text: "В случае если у Вас есть проблемы с приложением Вы можете <a href='tg://resolve?domain=AniLibriaTP'>написать в техподдержку в telegram</a>"
                            onLinkActivated: {
                                Qt.openUrlExternally(link);
                            }
                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.NoButton
                                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                            }
                        }
                    }

                    Rectangle {
                        color: "transparent"
                        width: 15
                        Layout.alignment: Qt.AlignCenter
                        Layout.fillWidth: true
                        Layout.preferredHeight: 15

                        Text {
                            anchors.centerIn: parent
                            font.pixelSize: 14
                            wrapMode: Text.WordWrap
                            width: parent.width
                            horizontalAlignment: Text.AlignHCenter
                            text: "Весь материал в приложении представлен исключительно для домашнего ознакомительного просмотра."
                        }
                    }

                    Rectangle {
                        color: "transparent"
                        width: 400
                        Layout.alignment: Qt.AlignCenter
                        Layout.fillWidth: true
                        Layout.preferredHeight: 15

                        Text {
                            anchors.centerIn: parent
                            font.pixelSize: 14
                            wrapMode: Text.WordWrap
                            width: parent.width
                            textFormat: Text.RichText
                            linkColor: "#b32121"
                            horizontalAlignment: Text.AlignHCenter
                            text: "В случаях нарушения авторских прав - обращайтесь на почту <a href='mailto:lupin@anilibria.tv'>lupin@anilibria.tv</a>"
                            onLinkActivated: {
                                Qt.openUrlExternally(link);
                            }
                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.NoButton
                                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                            }
                        }
                    }

                    Rectangle {
                        color: "transparent"
                        width: 400
                        Layout.alignment: Qt.AlignCenter
                        Layout.topMargin: 10
                        Layout.fillWidth: true
                        Layout.preferredHeight: 20

                        Text {
                            anchors.centerIn: parent
                            font.pixelSize: 18
                            linkColor: "#b32121"
                            text: "Дизайн:"
                        }
                    }

                    Rectangle {
                        color: "transparent"
                        width: 400
                        Layout.alignment: Qt.AlignCenter
                        Layout.fillWidth: true
                        Layout.topMargin: 8
                        Layout.preferredHeight: 19

                        Text {
                            anchors.centerIn: parent
                            font.pixelSize: 18
                            linkColor: "#b32121"
                            text: "Sebastian Wilde"
                        }
                    }

                    Rectangle {
                        color: "transparent"
                        width: 400
                        Layout.alignment: Qt.AlignCenter
                        Layout.topMargin: 10
                        Layout.fillWidth: true
                        Layout.preferredHeight: 20

                        Text {
                            anchors.centerIn: parent
                            font.pixelSize: 18
                            linkColor: "#b32121"
                            text: "Разработка:"
                        }
                    }

                    Rectangle {
                        color: "transparent"
                        width: 400
                        Layout.alignment: Qt.AlignCenter
                        Layout.fillWidth: true
                        Layout.preferredHeight: 19
                        Layout.topMargin: 8

                        Text {
                            anchors.centerIn: parent
                            font.pixelSize: 18
                            linkColor: "#b32121"
                            text: "RAS"
                        }
                    }

                    Rectangle {
                        color: "red"
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                }
            }
        }
    }

}

