import QtQuick 2.12
import QtGraphicalEffects 1.0

Item {
    property alias iconColor: _colorOverlay.color
    property real iconWidth
    property real iconHeight
    property alias iconSource: _iconImage.source

    width: iconWidth
    height: iconHeight

    Image {
        id: _iconImage
        width: iconWidth
        height: iconHeight
        mipmap: true
    }

    ColorOverlay {
        id: _colorOverlay
        anchors.fill: _iconImage
        source: _iconImage
    }
}

