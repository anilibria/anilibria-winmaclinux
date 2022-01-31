import QtQuick 2.12
import QtQuick.Controls 2.12

CommonMenu {
    property alias menuModel: options.model

    Repeater {
        id: options
        delegate: CommonMenuItem {
            text: title
            onTriggered: {
                options.model.selectItem(itemIndex);
            }
        }
    }
}
