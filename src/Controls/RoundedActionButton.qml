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
import QtQuick.Controls.Material 2.0
import "../Theme"

Rectangle {
    id: root
    color: "transparent"
    width: buttonTitle.width + 34
    height: 34
    property alias text: buttonTitle.text
    property alias textSize: buttonTitle.fontPointSize
    property bool buttonHovered: false
    property bool buttonEnabled: true
    property bool disableHover: false

    signal clicked()

    Rectangle {

        anchors.fill: parent
        anchors.leftMargin: 8
        anchors.rightMargin: 8
        anchors.topMargin: 4
        anchors.bottomMargin: 4
        color: root.buttonEnabled ? (root.buttonHovered ? ApplicationTheme.roundedButtonHovered : ApplicationTheme.roundedButtonBackground) : ApplicationTheme.roundedButtonBackgroundDisabled
        border.color: "transparent"
        border.width: 1
        radius: 18

        PlainText {
            id: buttonTitle
            fontPointSize: 11
            anchors.centerIn: parent
            color: ApplicationTheme.roundedButtonForeground
        }

        MouseArea {
            hoverEnabled: !disableHover
            anchors.fill: parent
            onEntered: {
                root.buttonHovered = true;
            }
            onExited: {
                root.buttonHovered = false;
            }
            onPressed: {
                if (!root.buttonEnabled) return;

                root.clicked();
            }
        }
    }

}

