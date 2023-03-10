import QtQuick 2.12
import QtQuick.Controls 2.12

Switch {
    property string tooltipMessage

    ToolTip.delay: 1000
    ToolTip.visible: hovered && tooltipMessage
    ToolTip.text: "<font color='" + applicationThemeViewModel.currentItems.colorTooltipText + "'>" + tooltipMessage + "</font>"
}
