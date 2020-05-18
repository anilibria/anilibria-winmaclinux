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

Item {
    id: rootItem
    signal pressed()
    property string iconSource
    height: 60


    MouseArea {
        id: mouseArea
        hoverEnabled: true
        Button {
            background: Rectangle {
                anchors.fill: parent
                color: mouseArea.containsMouse ? "lightgray" : "transparent"
                opacity: .6
            }
            leftPadding: 7
            topPadding: 10
            bottomPadding: 10
            icon.color: "transparent"
            icon.source: iconSource
            onClicked: {
                rootItem.pressed();
            }
        }
    }
}

