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
import "../Theme"

Page {
    id: aboutPage
    anchors.fill: parent

    signal navigateFrom()
    signal navigateTo()

    background: Rectangle {
        color: ApplicationTheme.pageBackground
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
            Rectangle {
                color: ApplicationTheme.pageVerticalPanel
                width: 40
                Layout.fillHeight: true
                Column {
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

                            MouseArea {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                width: 200
                                height: 200
                                onPressed: {
                                    anilibriaLogoEffect.visible = true;
                                }
                            }

                            Image {
                                id: anilibriaLogoImage
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                source: assetsLocation.iconsPath + "anilibrialogodefault.svg"
                                mipmap: true
                                width: 200
                                height: 200
                            }
                            ShaderEffect {
                                id: anilibriaLogoEffect
                                visible: false
                                width: 200
                                height: 200
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                property variant source: anilibriaLogoImage
                                property variant sourceNoise: anilibriaLogoImage
                                property real scratchAmount: 0.7
                                property real noiseAmount: 0
                                property var randomCoord1: Qt.vector2d(0.1, 0)
                                property var randomCoord2: Qt.vector2d(0, 0.1)
                                property real frame: 0

                                fragmentShader: "
                                    varying highp vec2 qt_TexCoord0;
                                    uniform sampler2D source;
                                    uniform sampler2D sourceNoise;
                                    uniform float scratchAmount;
                                    uniform float noiseAmount;
                                    uniform vec2 randomCoord1;
                                    uniform vec2 randomCoord2;
                                    uniform float frame;
                                    vec4 processColor(vec2 uv) {
                                        float scratchAmountInv = 1.0 / scratchAmount;
                                        vec4 color = texture2D(source, uv);
                                        vec2 sc = frame * vec2(0.001, 0.4);
                                        sc.x = fract(uv.x + sc.x);
                                        float scratch = texture2D(sourceNoise, sc).r;
                                        scratch = 2.0 * scratch * scratchAmountInv;
                                        scratch = 1.0 - abs(1.0 - scratch);
                                        scratch = max(float(0.0), scratch);
                                        color.r += scratch;
                                        color.g += scratch;
                                        color.b += scratch;
                                        vec2 rCoord = (((uv + randomCoord1) + randomCoord2) * 0.33);
                                        vec3 rand = vec3(texture2D(sourceNoise, rCoord));
                                        if (noiseAmount > (rand).r) {
                                            ((color).rgb = vec3 ((0.1 + ((rand).b * 0.4))));
                                        }
                                        float gray = dot(color, vec4(0.3, 0.59, 0.11, 0));
                                        color = vec4((gray * vec3(0.9, 0.8, 0.6)), 1);
                                        vec2 dist = (vec2 (0.5) - uv);
                                        float fluc = (((randomCoord2).x * 0.04) - 0.02);
                                        color.rgb *= vec3(((0.4 + fluc) - dot(dist, dist)) * 2.8);
                                        return color;
                                    }
                                    void main() {
                                       gl_FragColor = processColor(qt_TexCoord0);
                                    }"
                            }
                            Timer {
                                id: anilibrialogoeffect
                                running: anilibriaLogoEffect.visible
                                interval: 100
                                repeat: true
                                property bool negative: false
                                onTriggered: {
                                    if (anilibriaLogoEffect.frame > 2.5) negative = true;
                                    if (anilibriaLogoEffect.frame < 0) negative = false;

                                    anilibriaLogoEffect.frame += negative ? -0.1 : 0.1;
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
                                    "QtAV <a href='https://github.com/wang-bin/QtAV/blob/master/lgpl-2.1.txt'>license</a><br>"
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

