import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import "../Controls"
import "../Theme"

Page {
    id: myAnilibriaPage
    anchors.fill: parent

    signal navigateFrom()
    signal navigateTo()

    background: Rectangle {
        color: ApplicationTheme.pageBackground
    }

    Loader {
        anchors.fill: parent
        sourceComponent: myAnilibriaPage.visible ? myAnilibriaContent : null
    }

    Component {
        id: myAnilibriaContent

        RowLayout {
            id: panelContainer
            anchors.fill: parent
            spacing: 0
            Rectangle {
                color: ApplicationTheme.pageVerticalPanel
                width: 40
                Layout.fillHeight: true
                Column {
                    IconButton {
                        height: 45
                        width: 40
                        overlayVisible: false
                        iconPath: assetsLocation.iconsPath + "coloreddrawer.svg"
                        iconWidth: 28
                        iconHeight: 28
                        onButtonPressed: {
                            drawer.open();
                        }
                    }
                }
            }

            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                spacing: 2

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 45
                    height: 45
                    color: ApplicationTheme.pageUpperPanel

                    RoundedActionButton {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: openAllButton.right
                        width: 160
                        textSize: 10
                        text: "Закрыть все"
                        onClicked: {
                            myAnilibriaViewModel.myList.setNotVisibleAllMarks(true);
                        }
                    }

                    RoundedActionButton {
                        id: openAllButton
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        width: 160
                        textSize: 10
                        text: "Открыть все"
                        onClicked: {
                            myAnilibriaViewModel.myList.setNotVisibleAllMarks(false);
                        }
                    }

                    IconButton {
                        id: sectionVisibilities
                        anchors.right: parent.right
                        height: 45
                        width: 40
                        overlayVisible: false
                        iconPath: assetsLocation.iconsPath + "videos.svg"
                        iconWidth: 28
                        iconHeight: 28
                        tooltipMessage: "Изменить видимость секций станицы"
                        onButtonPressed: {
                            sectionVisibilitiesPopup.open();
                        }

                        Popup {
                            id: sectionVisibilitiesPopup
                            x: -420
                            width: 400
                            height: 200
                            modal: true
                            focus: true
                            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                            ListView {
                                id: allSectionList
                                anchors.fill: parent
                                clip: true
                                model: myAnilibriaViewModel.allSections
                                delegate: Item {
                                    width: allSectionList.width
                                    height: 40

                                    CheckBox {
                                        checked: sectionSelected
                                        text: sectionTitle
                                    }
                                }
                                ScrollBar.vertical: ScrollBar {
                                    active: true
                                }
                            }
                        }
                    }
                }

                ListView {
                    id: scrollview
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    clip: true
                    model: myAnilibriaViewModel.myList
                    delegate: Component {
                        Loader {
                            source: "./MyAnilibria/" + componentId + ".qml"
                        }
                    }
                    ScrollBar.vertical: ScrollBar {
                        active: true
                    }
                }
            }
        }
    }
}

