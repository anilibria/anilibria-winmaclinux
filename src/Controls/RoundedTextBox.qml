import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: _roundedTextBox
    color: "transparent"

    property alias textContent: textBox.text
    property string placeholder: ""

    signal completeEditing()

    CommonTextField {
        id: textBox
        anchors.fill: parent
        font.pointSize: 10
        selectByMouse: true
        placeholderText: placeholder
        onTextChanged: {
            if (completeTimer.running) {
                completeTimer.restart();
                return;
            }

            completeTimer.start();
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
        }
    }
}
