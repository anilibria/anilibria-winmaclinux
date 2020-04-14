import QtQuick 2.12
import QtQuick.Controls 2.12

Popup {
    id: messagePopup
    property alias header: popupHeader.text
    property alias message: popupMessage.text
    property alias content: innerSlotContainer.children

    x: window.width / 2 - 225
    y: window.height / 2 - 200
    width: 450
    height: 150
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    Column {
        width: parent.width
        spacing: 10
        Text {
            id: popupHeader
            width: messagePopup.width - 20
            font.pixelSize: 14
            font.bold: true
            elide: Text.ElideRight
        }

        Rectangle {
            width: messagePopup.width - 20
            height: messagePopup.height - 40
            Text {
                id: popupMessage
                width: parent.width
                height: 60
                font.pixelSize: 12
                wrapMode: Text.WordWrap
            }

            Rectangle {
                id: innerSlotContainer
                anchors.top: popupMessage.bottom
                width: parent.width
                height: 30
            }
        }
    }
}
