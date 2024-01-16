import QtQuick 2.12

Rectangle {
    id: _container
    color: isPressed ? applicationThemeViewModel.toggleButtonHovered : (isChecked ? applicationThemeViewModel.toggleButtonChecked : applicationThemeViewModel.toggleButtonNotChecked)

    property alias text: buttonText.text
    property bool isChecked: false
    property bool isPressed: false

    signal buttonClicked();
    signal buttonAlreadyClicked();

    Text {
        id: buttonText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color: applicationThemeViewModel.plainTextColor
        font.family: userConfigurationViewModel.textFont
        font.pointSize: 10
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            if (isChecked) {
                _container.buttonAlreadyClicked();
            } else {
                _container.buttonClicked();
            }
        }
        onEntered: {
            _container.isPressed = true;
        }
        onExited: {
            _container.isPressed = false;
        }
    }
}
