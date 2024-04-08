import QtQuick 2.12

Item {
    property var source: null

    Text {
        anchors.fill: parent
        text: "Стриминг экрана для этого плеера не поддерживается"
        wrapMode: Text.WordWrap
        maximumLineCount: 3
        elide: Text.ElideRight
    }
}
