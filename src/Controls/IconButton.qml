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
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.0

Item {
    id: _button
    property alias iconColor: colorOverlay.color
    property alias iconPath: iconImage.source
    property alias iconWidth: iconImage.width
    property alias iconHeight: iconImage.height
    property var hoverColor: "lightgray"
    property bool hovered: false

    signal buttonPressed()
    signal buttonHoverEnter()
    signal buttonHoverExit()

    MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: parent
        onEntered: {
            imageRectangle.color = _button.hoverColor;
            buttonHoverEnter();
            hovered = true;
        }
        onExited: {
            imageRectangle.color = "transparent";
            buttonHoverExit();
            hovered = false;
        }
        onClicked: {
            _button.buttonPressed();
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
            width: 29
            height: 29
            mipmap: true
        }

        ColorOverlay {
            id: colorOverlay
            anchors.fill: iconImage
            source: iconImage
            color: "white"
        }
    }
}
