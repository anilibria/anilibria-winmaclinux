import QtQuick 2.12
import QtQuick.Controls 2.12

Image {
    id: root
    mipmap: true

    property string tooltipMessage
    property bool tooltipMessageVisible

    ToolTip.delay: 2000
    ToolTip.visible: tooltipMessageVisible
    ToolTip.text: "<font color='" + applicationThemeViewModel.currentItems.colorTooltipText + "'>" + tooltipMessage + "</font>"
}
