import QtQuick 2.12

Item {
    property alias title: titleText.text

    Image {
        id: icon
        anchors.centerIn: parent
        width: 70
        height: 70
        source: assetsLocation.iconsPath + "emptybox.svg"
    }
    Text {
        id: titleText
        anchors.top: icon.bottom
        anchors.horizontalCenter: icon.horizontalCenter
    }
}
