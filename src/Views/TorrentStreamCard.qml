import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../Controls"

ColumnLayout {
    id: cardContainer
    anchors.fill: parent
    spacing: 0
    Rectangle {
        color: applicationThemeViewModel.currentItems.pageBackground
        Layout.fillWidth: true
        Layout.fillHeight: true

        Rectangle {
            width: 42
            height: parent.height
            anchors.right: parent.right
            color: applicationThemeViewModel.currentItems.pageUpperPanel
        }


    }
}
