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

Rectangle {
    id: _container
    color: isPressed ? "gray" : (isChecked ? "#9e2323" : (applicationThemeViewModel.basedOnDark ? "darkgrey" : "lightgray"))

    property alias text: buttonText.text
    property bool isChecked: false
    property bool isPressed: false

    signal buttonClicked();

    Text { font.family: "Helvetica"; font.pointSize: 13; font.bold: true }

    Text {
        id: buttonText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color: applicationThemeViewModel.plainTextColor
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            _container.buttonClicked();
        }
        onEntered: {
            _container.isPressed = true;
        }
        onExited: {
            _container.isPressed = false;
        }
    }
}
