import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import "../Controls"

Page {
    id: myAnilibriaPage
    anchors.fill: parent

    signal navigateFrom()
    signal navigateTo()

    background: Rectangle {
        color: applicationThemeViewModel.pageBackground
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

                ColumnLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    spacing: 2

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 45
                        height: 45
                        color: applicationThemeViewModel.pageUpperPanel

                        PlainText {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            visible: releasesViewModel.synchronizationEnabled
                            fontPointSize: 12
                            text: "Выполняется синхронизация..."
                        }

                        RoundedActionButton {
                            id: makeStartPageButton
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

                        CommonTextField {
                            id: filterTextField
                            anchors.left: makeStartPageButton.right
                            anchors.leftMargin: 10
                            width: 250
                            selectByMouse: true
                            placeholderText: "Введите название релиза"
                            onTextChanged: {
                                myAnilibriaSearchListModel.filter = text;
                            }

                            FilterPanelIconButton {
                                visible: filterTextField.text.length
                                anchors.right: parent.right
                                anchors.top: parent.top
                                anchors.topMargin: 6
                                width: 20
                                height: 20
                                iconPath: applicationThemeViewModel.currentItems.iconClearTextControl
                                iconWidth: 16
                                iconHeight: 16
                                tooltipMessage: "Очистить фильтр"
                                onButtonPressed: {
                                    filterTextField.text = "";
                                }
                            }
                        }

                        IconButton {
                            id: openAllButton
                            anchors.right: closeAllButton.left
                            height: 45
                            width: 40
                            iconPath: applicationThemeViewModel.currentItems.iconMyAnilibriaButtonPlus
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
                            iconPath: applicationThemeViewModel.currentItems.iconMyAnilibriaButtonMinus
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
                            iconPath: applicationThemeViewModel.currentItems.iconMyAnilibriaButtonSortSections
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
                                height: 480
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
                    color: applicationThemeViewModel.pageBackground
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

            Item {
                id: findedReleasesContainer
                x: filterTextField.x
                y: filterTextField.y + filterTextField.height
                width: 400
                height: findedReleases.height
                visible: myAnilibriaSearchListModel.hasItems
                Rectangle {
                    anchors.fill: parent
                    color: applicationThemeViewModel.panelBackground
                    radius: 8
                }

                ListView {
                    id: findedReleases
                    anchors.top: parent.top
                    anchors.topMargin: 4
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    width: parent.width
                    height: contentHeight
                    model: myAnilibriaSearchListModel
                    delegate: Item {
                        width: findedReleases.width
                        height: 90

                        Rectangle {
                            id: hoveredRectangle
                            visible: false
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.leftMargin: -4
                            anchors.right: parent.right
                            anchors.rightMargin: -4
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 10
                            color: applicationThemeViewModel.panelBackgroundShadow
                        }

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onEntered: {
                                hoveredRectangle.visible = true;
                            }
                            onExited: {
                                hoveredRectangle.visible = false;
                            }
                            onPressed: {
                                mainViewModel.selectPage("videoplayer");
                                onlinePlayerViewModel.quickSetupForSingleRelease(releaseId);
                                filterTextField.text = "";
                            }
                        }

                        Image {
                            id: posterImage
                            anchors.left: parent.left
                            source: localStorage.getReleasePosterPath(releaseId, poster)
                            sourceSize: Qt.size(350, 500)
                            fillMode: Image.PreserveAspectCrop
                            width: 50
                            height: 78
                            mipmap: true
                        }

                        Column {
                            height: parent.height
                            width: parent.width - posterImage.width - 4
                            anchors.left: posterImage.right
                            anchors.leftMargin: 4

                            Item {
                                width: parent.width
                                height: 45

                                AccentText {
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: 4
                                    verticalAlignment: Text.AlignVCenter
                                    width: 280
                                    height: 20
                                    fontPointSize: 10
                                    text: title
                                    maximumLineCount: 1
                                    elide: Text.ElideRight
                                    wrapMode: Text.NoWrap
                                }

                                PlainText {
                                    anchors.right: parent.right
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: 4
                                    fontPointSize: 9
                                    text: year
                                }
                            }

                            Item {
                                width: parent.width
                                height: 45

                                PlainText {
                                    anchors.top: parent.top
                                    width: 280
                                    height: 30
                                    fontPointSize: 9
                                    text: originalName
                                    maximumLineCount: 1
                                    elide: Text.ElideRight
                                    wrapMode: Text.NoWrap
                                }

                                PlainText {
                                    anchors.top: parent.top
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.right: parent.right
                                    fontPointSize: 9
                                    text: season
                                }
                            }
                        }
                    }
                }
            }

            Connections {
                target: myAnilibriaViewModel
                function onNeedRestoreScroll() {
                    scrollview.contentY = myAnilibriaViewModel.restoreScroll;
                }
            }

            Component.onCompleted: {
                myAnilibriaViewModel.refreshAllSectionsModels();
            }
        }
    }
}

