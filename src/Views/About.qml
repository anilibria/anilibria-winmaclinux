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
import "../Controls"

Page {
    id: aboutPage
    anchors.fill: parent

    signal navigateFrom()
    signal navigateTo()

    background: Rectangle {
        color: applicationThemeViewModel.pageBackground
    }

    Loader {
        anchors.fill: parent
        sourceComponent: aboutPage.visible ? aboutContent : null
    }

    Component {
        id: aboutContent

        RowLayout {
            id: panelContainer
            anchors.fill: parent
            spacing: 0

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
                                id: anilibriaLogoImage
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                source: assetsLocation.iconsPath + "anilibrialogodefault.svg"
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

                            PlainText {
                                anchors.centerIn: parent
                                fontPointSize: 15
                                text: "AniLibria.Qt"
                            }
                        }

                        Rectangle {
                            color: "transparent"
                            width: 400
                            height: 40
                            Layout.alignment: Qt.AlignCenter
                            Layout.fillWidth: true
                            Layout.preferredHeight: 40

                            LinkedText {
                                anchors.centerIn: parent
                                fontPointSize: 14
                                text: "<a href='https://www.anilibria.tv/'>https://www.anilibria.tv/</a>"
                            }
                        }

                        Rectangle {
                            color: "transparent"
                            width: 400
                            height: 40
                            Layout.alignment: Qt.AlignCenter
                            Layout.fillWidth: true
                            Layout.preferredHeight: 40

                            PlainText {
                                anchors.centerIn: parent
                                fontPointSize: 12
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

                            AccentText {
                                anchors.centerIn: parent
                                fontPointSize: 14
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

                            LinkedText {
                                anchors.centerIn: parent
                                fontPointSize: 10
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignHCenter
                                text: "В случае если у Вас есть проблемы с приложением Вы можете <a href='https://t.me/Libria911Bot'>написать в техподдержку в telegram</a>"
                            }
                        }

                        Rectangle {
                            color: "transparent"
                            width: 15
                            Layout.alignment: Qt.AlignCenter
                            Layout.fillWidth: true
                            Layout.preferredHeight: 15

                            PlainText {
                                anchors.centerIn: parent
                                fontPointSize: 10
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

                            LinkedText {
                                anchors.centerIn: parent
                                fontPointSize: 10
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignHCenter
                                text: "В случаях нарушения авторских прав - обращайтесь на почту <a href='mailto:lupin@anilibria.tv'>lupin@anilibria.tv</a>"
                            }
                        }

                        Rectangle {
                            color: "transparent"
                            width: 400
                            Layout.alignment: Qt.AlignCenter
                            Layout.topMargin: 10
                            Layout.fillWidth: true
                            Layout.preferredHeight: 20

                            PlainText {
                                anchors.centerIn: parent
                                fontPointSize: 12
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

                            PlainText {
                                anchors.centerIn: parent
                                fontPointSize: 11
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
                            PlainText {
                                anchors.centerIn: parent
                                fontPointSize: 12
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

                            LinkedText {
                                anchors.centerIn: parent
                                fontPointSize: 11
                                text: "<a href='https://github.com/anilibria/anilibria-winmaclinux/graphs/contributors'>Контрибьюторы проекта</a>"
                            }
                        }

                        Rectangle {
                            color: "transparent"
                            width: 400
                            Layout.alignment: Qt.AlignCenter
                            Layout.topMargin: 4
                            Layout.bottomMargin: 10
                            Layout.fillWidth: true
                            Layout.preferredHeight: 20

                            LinkedText {
                                anchors.centerIn: parent
                                fontPointSize: 12
                                text: "Иконки:"
                            }
                        }

                        Rectangle {
                            color: "transparent"
                            width: 15
                            Layout.alignment: Qt.AlignCenter
                            Layout.fillWidth: true
                            Layout.preferredHeight: 100

                            LinkedText {
                                anchors.centerIn: parent
                                fontPointSize: 10
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignHCenter
                                text: "Icons made by <a href='https://www.flaticon.com/authors/dave-gandy'>Dave Gandy</a> from <a href='https://www.flaticon.com/'>www.flaticon.com</a><br>" +
                                      "Icons made by <a href='https://www.flaticon.com/authors/icongeek26'>Icongeek26</a> from <a href='https://www.flaticon.com/'>www.flaticon.com</a><br>" +
                                      "Icons made by <a href='https://www.flaticon.com/authors/srip'>srip</a> from <a href='https://www.flaticon.com/'>www.flaticon.com</a><br>" +
                                      "Icons made by <a href='https://www.flaticon.com/authors/swifticons'>Swifticons</a> from <a href='https://www.flaticon.com/'>www.flaticon.com</a><br>" +
                                      "Icons made by <a href='https://www.flaticon.com/authors/google'>Google</a> from <a href='https://www.flaticon.com/'>www.flaticon.com</a><br>" +
                                      "Icons made by <a href='https://www.flaticon.com/authors/icongeek26'>Icongeek26</a> from <a href='https://www.flaticon.com/'>www.flaticon.com</a><br>" +
                                      "Icons made by <a href='https://roundicons.com/'>Roundicons</a> from <a href='https://www.flaticon.com/' title='Flaticon'>www.flaticon.com</a>"
                            }
                        }

                        Rectangle {
                            color: "transparent"
                            width: 100
                            Layout.alignment: Qt.AlignCenter
                            Layout.topMargin: 10
                            Layout.bottomMargin: 10
                            Layout.fillWidth: true
                            Layout.preferredHeight: 20

                            LinkedText {
                                anchors.centerIn: parent
                                fontPointSize: 12
                                text: "Используемые компоненты:"
                            }
                        }

                        Rectangle {
                            color: "transparent"
                            width: 15
                            Layout.alignment: Qt.AlignCenter
                            Layout.fillWidth: true
                            Layout.preferredHeight: 50

                            LinkedText {
                                anchors.centerIn: parent
                                fontPointSize: 10
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignHCenter
                                text: "Qt <a href='https://github.com/qt/qt5/blob/dev/LICENSE.FDL'>license</a><br>" +
                                    "QtAV <a href='https://github.com/wang-bin/QtAV/blob/master/lgpl-2.1.txt'>license</a><br>
                                     VLC <a href='https://www.gnu.org/licenses/gpl-2.0.html#SEC3'>license</a>"
                            }
                        }


                        Rectangle {
                            color: "transparent"
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }
                    }
                }
            }
        }

    }
}

