import QtQuick 2.12

Item {
    id: root
    property alias posterSource: imagePoster.source
    property string emptyBorderBackground: "transparent"

    Image {
        id: imagePoster
        sourceSize: Qt.size(350, 500)
        fillMode: Image.PreserveAspectCrop
        width: root.width
        height: root.height
        clip: true
    }

    Rectangle {
        anchors.centerIn: root
        width: root.width
        height: root.height
        border.color: root.emptyBorderBackground
        border.width: 4
        color: "transparent"
    }

    Rectangle {
        anchors.centerIn: root
        width: root.width
        height: root.height
        border.color: root.emptyBorderBackground
        border.width: 4
        radius: 12
        color: "transparent"
    }

    Rectangle {
        anchors.centerIn: root
        width: root.width - 6
        height: root.height - 6
        border.color: applicationThemeViewModel.posterBorder
        border.width: 1
        color: "transparent"
        radius: 11
        clip: true

        Rectangle {
            anchors.fill: parent
            color: applicationThemeViewModel.currentItems.colorPosterFilter
            radius: 12
        }
    }
}

