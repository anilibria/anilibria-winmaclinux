import QtQuick 2.12
import QtQuick.Controls 2.12
import CustomStyle 1.0
import "../Controls"

IconButton {
    id: root
    property string tooltipMessage
    height: 45
    width: 40
    hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
    iconWidth: 30
    iconHeight: 30

    ToolTip {
        delay: 2000
        visible: hovered && tooltipMessage
        text: tooltipMessage
        CustomStyle.tooltipColor: window.tooltipColor
        CustomStyle.colorTooltipText: window.tooltipColorText
    }
}
