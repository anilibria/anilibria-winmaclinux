import QtQuick 2.15
import QtQuick.Layouts 1.15
import "../Controls"

Item {
    id: root
    visible: root.isVisible
    enabled: root.isVisible
    anchors.fill: parent
    property bool isVisible: false

    MouseArea {
        visible: root.isVisible
        enabled: root.isVisible
        anchors.fill: parent
        onPressed: {
            root.isVisible = false;
        }
    }

    Rectangle {
        visible: root.isVisible
        enabled: root.isVisible
        color: "black"
        anchors.fill: parent
        opacity: .5
    }

    Image {
        id: previewReleasePoster
        visible: root.isVisible
        enabled: root.isVisible
        anchors.centerIn: parent
        source: releasesViewModel.isOpenedCard ? localStorage.getReleasePosterPath(releasesViewModel.openedReleaseId, releasesViewModel.openedReleasePoster) : ""
        width: 350
        height: 500
    }
}
