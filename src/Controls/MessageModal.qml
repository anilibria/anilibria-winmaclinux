import QtQuick 2.15
import QtQuick.Controls 2.15

DefaultPopup {
    id: messagePopup
    property alias header: popupHeader.text
    property alias message: popupMessage.text
    property alias messageHeight: popupMessage.height
    property alias content: innerSlotContainer.children

    x: (window.width / 2) - (width / 2)
    y: (window.height / 2) - (height / 2)
    width: 450
    height: 150
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    Column {
        width: parent.width
        spacing: 10

        AccentText {
            id: popupHeader
            width: messagePopup.width - 20
            fontPointSize: 11
            font.bold: true
            elide: Text.ElideRight
        }

        Rectangle {
            color: "transparent"
            width: messagePopup.width - 20
            height: messagePopup.height - 40

            LinkedText {
                id: popupMessage
                width: parent.width
                height: 60
                fontPointSize: 9
                wrapMode: Text.WordWrap
            }

            Rectangle {
                id: innerSlotContainer
                color: "transparent"
                anchors.top: popupMessage.bottom
                width: parent.width
                height: 30
            }
        }
    }
}
