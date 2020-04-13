import QtQuick 2.12
import QtQuick.Controls 2.12

MenuItem {
    property bool notVisible: false

    width: parent.width
    height: notVisible ? 0 : undefined
    visible: !notVisible
    font.pixelSize: 14
}
