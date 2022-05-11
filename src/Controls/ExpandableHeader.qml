import QtQuick 2.12
import "../Theme"

Rectangle {
    id: root
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
    property bool isFirst: false
    property bool isLast: false

    signal headerPressed();
    signal moveSection(int direction);

    AccentText {
        id: titleText
        anchors.left: parent.left
        anchors.leftMargin: 4
        anchors.verticalCenter: parent.verticalCenter
        fontPointSize: 12
    }

    IconButton {
        id: visibleContentButton
        width: 26
        height: parent.height - 4
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 4
        iconColor: ApplicationTheme.plainTextColor
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

    Row {
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: visibleContentButton.left

        IconButton {
            id: sortUpButton
            visible: !isFirst
            width: 26
            height: root.height - 4
            iconWidth: 22
            iconHeight: 22
            overlayVisible: false
            iconPath: assetsLocation.iconsPath + "coloredsortup.svg"
            onButtonPressed: {
                moveSection(1);
            }
        }

        IconButton {
            id: sortDownButton
            visible: !isLast
            width: 26
            height: root.height - 4
            iconWidth: 22
            iconHeight: 22
            overlayVisible: false
            iconPath: assetsLocation.iconsPath + "coloredsortdown.svg"
            onButtonPressed: {
                moveSection(-1);
            }
        }
    }

    MouseArea {
        height: parent.height
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 84
        onPressed: {
            headerPressed();
        }
    }
}
