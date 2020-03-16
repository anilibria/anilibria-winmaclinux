import QtQuick 2.12
import QtQuick.Controls 2.5

Item {
    id: rootItem
    signal pressed()
    property string iconSource
    height: 60


    MouseArea {
        id: mouseArea
        hoverEnabled: true
        Button {
            background: Rectangle {
                anchors.fill: parent
                color: mouseArea.containsMouse ? "lightgray" : "transparent"
                opacity: .6
            }
            leftPadding: 7
            topPadding: 10
            bottomPadding: 10
            icon.color: "transparent"
            icon.source: iconSource
            onClicked: {
                rootItem.pressed();
            }
        }
    }
}

