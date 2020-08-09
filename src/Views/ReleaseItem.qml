import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import "../Controls"
import "../Theme"

Rectangle {
    id: releaseItem
    width: 480
    height: 280
    radius: 10
    border.color: ApplicationTheme.selectedItem
    border.width: releaseItem.isSelected ? 3 : 0
    color: !compactModeSwitch.checked ? ApplicationTheme.panelBackground : "transparent"
    layer.enabled: !compactModeSwitch.checked
    layer.effect: DropShadow {
        transparentBorder: true
        horizontalOffset: 2
        verticalOffset: 2
        radius: 1
        samples: 3
        color: ApplicationTheme.panelBackgroundShadow
    }

    property var releaseModel: ({})
    property var favoriteReleases: []
    property bool isSelected: false

    signal leftClicked()
    signal rightClicked()
    signal addToFavorite(int id)
    signal removeFromFavorite(int id)
    signal watchRelease(int id, string videos)

    Rectangle {
        id: mask
        width: 180
        height: 260
        radius: 10
        visible: false
    }

    MouseArea {
        width: 480
        height: 280
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            if(mouse.button & Qt.RightButton) {
                releaseItem.rightClicked();
                return;
            } else {
                releaseItem.leftClicked();
            }
        }
    }
    Grid {
        anchors.topMargin: 10
        columnSpacing: 3
        rowSpacing: 3
        columns: !compactModeSwitch.checked ? 2 : 1
        rows: compactModeSwitch.checked ? 2 : 1
        bottomPadding: 4
        leftPadding: 4
        topPadding: 4
        rightPadding: 4

        Rectangle {
            visible: releaseModel.id > -1 && !compactModeSwitch.checked
            width: 182
            height: 272
            border.color: "#adadad"
            border.width: 1
            radius: 12

            Image {
                anchors.centerIn: parent
                source: localStorage.getReleasePosterPath(releaseModel.id, releaseModel.poster)
                sourceSize: Qt.size(350, 500)
                fillMode: Image.PreserveAspectCrop
                width: 180
                height: 270
                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: mask
                }
            }
        }

        Rectangle {
            visible: compactModeSwitch.checked
            width: 460
            height: 236
            color: "transparent"

            Rectangle {
                visible: releaseModel.id > -1
                anchors.horizontalCenter: parent.horizontalCenter
                width: 152
                height: 232
                border.color: "#adadad"
                border.width: 1
                radius: 12

                Image {
                    anchors.centerIn: parent
                    source: localStorage.getReleasePosterPath(releaseModel.id, releaseModel.poster)
                    sourceSize: Qt.size(150, 230)
                    fillMode: Image.PreserveAspectCrop
                    width: 150
                    height: 230
                    layer.enabled: true
                    layer.effect: OpacityMask {
                        maskSource: mask
                    }
                }
            }
        }

        Rectangle {
            visible: compactModeSwitch.checked
            width: 460
            height: 38
            color: "transparent"

            AccentText {
                fontPointSize: 10
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                width: 460
                leftPadding: 8
                topPadding: 6
                wrapMode: Text.WordWrap
                maximumLineCount: 2
                text: releaseModel.title
            }
        }

        Grid {
            visible: !compactModeSwitch.checked
            height: 280
            Layout.row: 1
            Layout.column: 1
            rows: 2
            columns: 1
            Column {
                id: gridItemtextContainer
                AccentText {
                    textFormat: Text.RichText
                    fontPointSize: 12
                    width: 280
                    leftPadding: 8
                    topPadding: 6
                    wrapMode: Text.WordWrap
                    maximumLineCount: 3
                    text: qsTr(releaseModel.title)
                }
                PlainText {
                    visible: releaseModel.id > -1
                    textFormat: Text.RichText
                    fontPointSize: 10
                    leftPadding: 8
                    topPadding: 4
                    text: qsTr("<b>Статус:</b> ") + qsTr(releaseModel.status)
                }
                PlainText {
                    visible: releaseModel.id > -1
                    fontPointSize: 10
                    leftPadding: 8
                    topPadding: 4
                    text: qsTr("<b>" + releaseModel.season + " " + releaseModel.year + "</b>")
                }
                PlainText {
                    visible: releaseModel.id > -1
                    textFormat: Text.RichText
                    fontPointSize: 10
                    leftPadding: 8
                    topPadding: 4
                    width: 280
                    wrapMode: Text.WordWrap
                    maximumLineCount: 2
                    text: qsTr("<b>Тип:</b> ") + qsTr(releaseModel.type)
                }
                PlainText {
                    visible: releaseModel.id > -1
                    fontPointSize: 10
                    leftPadding: 8
                    topPadding: 4
                    width: 280
                    wrapMode: Text.WordWrap
                    maximumLineCount: 2
                    text: qsTr("<b>Жанры:</b> ") + qsTr(releaseModel.genres)
                }
                PlainText {
                    visible: releaseModel.id > -1
                    fontPointSize: 10
                    leftPadding: 8
                    topPadding: 4
                    width: 280
                    wrapMode: Text.WordWrap
                    maximumLineCount: 2
                    text: qsTr("<b>Озвучка:</b> ") + qsTr(releaseModel.voices)
                }
                Row {
                    visible: releaseModel.id > -1
                    leftPadding: 8
                    topPadding: 4
                    ColoredIcon {
                        iconSource: '../Assets/Icons/online.svg'
                        iconWidth: 22
                        iconHeight: 22
                        iconColor: ApplicationTheme.plainTextColor
                    }
                    PlainText {
                        leftPadding: 4
                        rightPadding: 8
                        fontPointSize: 12
                        text: '' + releaseModel.countVideos + (releaseModel.countSeensSeries > 0 ? " <font color='" + (ApplicationTheme.isDarkTheme ? "white" : "green") + "'>(" + releaseModel.countSeensSeries + ")</font>  " : "")
                    }
                    ColoredIcon {
                        iconSource: '../Assets/Icons/utorrent.svg'
                        iconWidth: 22
                        iconHeight: 22
                        iconColor: ApplicationTheme.plainTextColor
                    }
                    PlainText {
                        leftPadding: 4
                        rightPadding: 4
                        fontPointSize: 12
                        text: '' + releaseModel.countTorrents
                    }
                }
            }
            Rectangle {
                visible: releaseModel.id > -1
                color: "transparent"
                height: 272 - gridItemtextContainer.height
                width: 280
                RoundedActionButton {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 20
                    text: "Быстрые действия"
                    onPressed: {
                        quickActions.open();
                    }
                    CommonMenu {
                        id: quickActions
                        width: 320

                        CommonMenuItem {
                            enabled: applicationSettings.userToken && !releaseItem.favoriteReleases.filter(a => a === releaseModel.id).length
                            text: "Добавить в избранное"
                            onPressed: {
                                quickActions.close();
                                releaseItem.addToFavorite(releaseModel.id);
                            }
                        }
                        CommonMenuItem {
                            enabled: applicationSettings.userToken && releaseItem.favoriteReleases.filter(a => a === releaseModel.id).length
                            text: "Удалить из избранного"
                            onPressed: {
                                quickActions.close();
                                releaseItem.removeFromFavorite(releaseModel.id);
                            }
                        }
                        CommonMenuItem {
                            text: "Смотреть"
                            onPressed: {
                                quickActions.close();
                                releaseItem.watchRelease(releaseModel.id, releaseModel.videos);
                            }
                        }
                    }
                }
                Row {
                    anchors.bottom: parent.bottom
                    leftPadding: 8
                    topPadding: 8
                    ColoredIcon {
                        iconSource: '../Assets/Icons/rating.svg'
                        iconWidth: 18
                        iconHeight: 20
                        iconColor: ApplicationTheme.plainTextColor
                    }
                    PlainText {
                        leftPadding: 4
                        rightPadding: 4
                        fontPointSize: 12
                        text: releaseModel.rating
                    }
                    ColoredIcon {
                        visible: releaseItem.favoriteReleases.filter(a => a === releaseModel.id).length
                        iconSource: '../Assets/Icons/favorite.svg'
                        iconWidth: 20
                        iconHeight: 20
                        iconColor: ApplicationTheme.headerTextColor
                    }
                    PlainText {
                        visible: releaseItem.favoriteReleases.filter(a => a === releaseModel.id).length
                        leftPadding: 4
                        color: ApplicationTheme.headerTextColor
                        fontPointSize: 12
                        text: "В избранном"
                    }
                }
            }
        }
    }
}
