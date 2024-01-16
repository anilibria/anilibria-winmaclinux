import QtQuick 2.12
import QtQuick.Controls 2.12
import CustomStyle 1.0

Image {
    id: root
    mipmap: true

    property string tooltipMessage
    property bool tooltipMessageVisible

    ToolTip {
        delay: 2000
        visible: tooltipMessageVisible && tooltipMessage
        text: tooltipMessage
        CustomStyle.tooltipColor: applicationThemeViewModel.currentItems.colorTooltipBackground
        CustomStyle.colorTooltipText: applicationThemeViewModel.currentItems.colorTooltipText
    }
}
