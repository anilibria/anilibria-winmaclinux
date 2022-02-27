import QtQuick 2.12

Rectangle {
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.margins: 4
    height: 30
    color: "transparent"
    border.color: "white"
    border.width: 1
    radius: 4

    property alias headerTitle: titleText.text
    property bool headerHided: false

    signal headerPressed();

    AccentText {
        id: titleText
        anchors.left: parent.left
        anchors.leftMargin: 4
        anchors.verticalCenter: parent.verticalCenter
        fontPointSize: 12
    }

    IconButton {
        width: 26
        height: parent.height - 4
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 4
        iconColor: "white"
        iconWidth: 22
        iconHeight: 22
        iconPath: assetsLocation.iconsPath + "arrowup.svg"
        transform: Rotation {
            id: rotationIconButton
            angle: headerHided ? 180 : 0
            origin.x: 13
            origin.y: 13
        }
        onButtonPressed: {
            headerPressed();
        }
    }

    MouseArea {
        anchors.fill: parent
        onPressed: {
            headerPressed();
        }
    }
}
