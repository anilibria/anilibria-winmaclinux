import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

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
    property bool allowedRightClick: false

    signal clicked()
    signal rightClicked()

    Rectangle {

        anchors.fill: parent
        anchors.leftMargin: 8
        anchors.rightMargin: 8
        anchors.topMargin: 4
        anchors.bottomMargin: 4
        color: root.buttonEnabled ? (root.buttonHovered ? applicationThemeViewModel.roundedButtonHovered : applicationThemeViewModel.roundedButtonBackground) : applicationThemeViewModel.roundedButtonBackgroundDisabled
        border.color: "transparent"
        border.width: 1
        radius: 18

        PlainText {
            id: buttonTitle
            fontPointSize: 11
            anchors.centerIn: parent
            color: root.buttonEnabled ? applicationThemeViewModel.roundedButtonForeground : applicationThemeViewModel.roundedButtonDisableForeground
        }

        MouseArea {
            hoverEnabled: !disableHover
            anchors.fill: parent
            acceptedButtons: root.allowedRightClick ? Qt.LeftButton | Qt.RightButton : Qt.LeftButton
            onEntered: {
                root.buttonHovered = true;
            }
            onExited: {
                root.buttonHovered = false;
            }
            onPressed: {
                if (!root.buttonEnabled) return;

                if (mouse.button == Qt.RightButton) {
                    root.rightClicked();
                } else {
                    root.clicked();
                }
            }
        }
    }

}

