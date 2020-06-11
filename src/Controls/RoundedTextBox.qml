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

Rectangle {
    id: _roundedTextBox
    height: 20
    radius: height / 2
    border.width: 1
    border.color: "#999"

    property alias textContent: edit.text
    property string placeholder: ""
    property alias fontSize: edit.font.pointSize

    signal completeEditing()

    Flickable {
        id: flick
        anchors.fill: parent
        anchors.leftMargin: 6
        anchors.topMargin: (_roundedTextBox.height / 2) - (edit.height / 2)
        anchors.rightMargin: 6
        contentWidth: edit.paintedWidth
        contentHeight: edit.paintedHeight
        clip: true

        function ensureVisible(r)
        {
            if (contentX >= r.x)
                contentX = r.x;
            else if (contentX+width <= r.x+r.width)
                contentX = r.x+r.width-width;
            if (contentY >= r.y)
                contentY = r.y;
            else if (contentY+height <= r.y+r.height)
                contentY = r.y+r.height-height;
        }

        TextEdit {
            id: edit
            width: flick.width
            text: textContent
            onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
            onTextChanged: {
                if (completeTimer.running) {
                    completeTimer.restart();
                    return;
                }

                completeTimer.start();
            }

            Text {
                text: placeholder
                color: "#aaa"
                font.pointSize: fontSize
                visible: placeholder && !edit.text
            }
            Keys.onReturnPressed: {
                //WORKAROUND: prevented an increase in the number of lines in the control
            }
        }
    }

    Timer {
        id: completeTimer
        repeat: false
        running: false
        interval: 1000
        onTriggered: {
            completeEditing();
            completeTimer.stop();
            edit.focus = false;
        }
    }
}
