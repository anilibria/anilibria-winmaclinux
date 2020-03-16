import QtQuick 2.12
import QtQuick.Controls 2.5

Rectangle {
    id: _roundedTextBox
    height: 20
    radius: height / 2
    border.width: 1
    border.color: "#999"

    property alias textContent: edit.text
    property string placeholder: ""
    property alias fontSize: edit.font.pixelSize

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
            focus: true
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
                font.pixelSize: fontSize
                visible: placeholder && !edit.text
            }
        }
    }

    Timer {
        id: completeTimer
        repeat: false
        running: false
        interval: 2000
        onTriggered: {
            completeEditing();
            completeTimer.stop();
        }
    }
}
