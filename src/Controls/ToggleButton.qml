import QtQuick 2.12

Rectangle {
    id: _container
    color: isPressed ? "gray" : (isChecked ? "#9e2323" :"lightgray")

    property alias text: buttonText.text
    property bool isChecked: false
    property bool isPressed: false

    signal buttonClicked();

    Text { font.family: "Helvetica"; font.pointSize: 13; font.bold: true }

    Text {
        id: buttonText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color: "black"
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
