import QtQuick 2.15

Item {
    id: root
    visible: osExtras.synchronizationStarted
    enabled: osExtras.synchronizationStarted
    width: 320
    height: 120

    Rectangle {
        anchors.fill: parent
        color: applicationThemeViewModel.panelBackground
        border.width: 1
        border.color: applicationThemeViewModel.plainTextColor
        radius: 8
    }

    LinkedText {
        anchors.fill: parent
        anchors.margins: 4
        wrapMode: Text.Wrap
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        fontPointSize: 10
        text: "<b>Выполняется синхронизация...</b><br />Процент выполнения: " + osExtras.synchronizationPercent + "%"
    }
}
