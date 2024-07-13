import QtQuick 2.15

Item {
    property alias title: titleText.text

    Image {
        id: icon
        anchors.centerIn: parent
        width: 70
        height: 70
        source: assetsLocation.iconsPath + "emptybox.svg"
    }
    PlainText {
        id: titleText
        anchors.top: icon.bottom
        fontPointSize: 9
        anchors.horizontalCenter: icon.horizontalCenter
    }
}
