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

        Item {
            anchors.fill: parent

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

                        PlainText {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            visible: releasesViewModel.synchronizationEnabled
                            fontPointSize: 12
                            text: "Выполняется синхронизация..."
                        }

                        RoundedActionButton {
                            buttonEnabled: userConfigurationViewModel.startPage !== "myanilibria"
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            width: 160
                            textSize: 10
                            text: "Сделать стартовой"
                            onClicked: {
                                userConfigurationViewModel.startPage = "myanilibria";
                            }
                        }

                        IconButton {
                            id: openAllButton
                            anchors.right: closeAllButton.left
                            height: 45
                            width: 40
                            overlayVisible: false
                            iconPath: assetsLocation.iconsPath + "plus.svg"
                            iconWidth: 28
                            iconHeight: 28
                            tooltipMessage: "Открыть все секции"
                            onButtonPressed: {
                                myAnilibriaViewModel.myList.setNotVisibleAllMarks(false);
                            }
                        }

                        IconButton {
                            id: closeAllButton
                            anchors.right: sectionVisibilities.left
                            height: 45
                            width: 40
                            overlayVisible: false
                            iconPath: assetsLocation.iconsPath + "minus.svg"
                            iconWidth: 28
                            iconHeight: 28
                            tooltipMessage: "Закрыть все секции"
                            onButtonPressed: {
                                myAnilibriaViewModel.myList.setNotVisibleAllMarks(true);
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

                            DefaultPopup {
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
                                            onCheckedChanged: {
                                                if (checked) {
                                                    myAnilibriaViewModel.selectSection(sectionId)
                                                } else {
                                                    myAnilibriaViewModel.deselectSection(sectionId)
                                                }
                                            }
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

            Item {
                visible: myAnilibriaViewModel.hoveredDescription.length
                enabled: false
                anchors.left: parent.left
                anchors.leftMargin: 42
                anchors.bottom: parent.bottom
                anchors.rightMargin: 2
                height: 105
                width: parent.width / 2

                Rectangle {
                    anchors.fill: parent
                    color: ApplicationTheme.pageBackground
                    opacity: 0.8
                }

                PlainText {
                    anchors.fill: parent
                    fontPointSize: 11
                    text: myAnilibriaViewModel.hoveredDescription
                    wrapMode: Text.WordWrap
                    textFormat: Text.RichText
                    maximumLineCount: 5
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

    }
}

