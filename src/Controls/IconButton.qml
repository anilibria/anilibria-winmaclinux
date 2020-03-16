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

    signal buttonPressed()

    MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: parent
        onEntered: {
            imageRectangle.color = _button.hoverColor;//"lightgray";
        }
        onExited: {
            imageRectangle.color = "transparent";
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
