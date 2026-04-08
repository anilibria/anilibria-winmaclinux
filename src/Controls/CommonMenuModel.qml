import QtQuick 2.15
import QtQuick.Controls 2.15

CommonMenu {
    property alias menuModel: options.model

    signal beforeAction()

    Repeater {
        id: options
        delegate: CommonMenuItem {
            text: title
            onTriggered: {
                beforeAction();

                options.model.selectItem(itemIndex);
            }
        }
    }
}
