import QtQuick 2.15

Text {
    property real fontPointSize: 0

    font.pointSize: fontPointSize + windowSettings.dpiSeparation
    linkColor: applicationThemeViewModel.linkTextColor
    color: applicationThemeViewModel.plainTextColor
    onLinkActivated: {
        Qt.openUrlExternally(link);
    }
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.NoButton
        cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
    }
}
