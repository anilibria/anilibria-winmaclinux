import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import "../Theme"
import "../Controls"

Page {
    id: root
    anchors.fill: parent

    background: Rectangle {
        color: ApplicationTheme.pageBackground
    }

    signal navigateFrom()
    signal navigateTo()

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
                    iconColor: "white"
                    iconPath: "../Assets/Icons/menu.svg"
                    iconWidth: 29
                    iconHeight: 29
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

                Switch {
                    id: multupleMode
                    anchors.left: parent.left
                    onCheckedChanged: {
                        page.selectMode = checked;
                        if (!checked) {
                            page.selectedReleases = [];
                        } else {
                            page.openedRelease = null;
                        }
                    }
                    ToolTip.delay: 1000
                    ToolTip.visible: multupleMode.hovered
                    ToolTip.text: "Данный переключатель влияет на поведение при клике ЛКМ на релизах в списке\nОдиночный выбор позволяет открывать карточку с подробной информацией\nМножественный выбор позволяет выбрать несколько релизов и выполнять действия\nЧтобы переключать его можно использовать клик ПКМ в области списка релизов"
                }
                PlainText {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 4
                    anchors.left: multupleMode.right
                    fontPointSize: 12
                    text: multupleMode.checked ? "Множественный выбор" : "Одиночный выбор"

                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            multupleMode.checked = !multupleMode.checked;
                        }
                    }
                }
            }

            ListModel {
                id: releases
            }

            ListView {
                id: listViewReleases
                Layout.fillHeight: true
                Layout.fillWidth: true
                orientation: ListView.Horizontal
            }
        }
    }

    onNavigateTo: {
        root.releases = JSON.parse(localStorage.getCinemahallReleases());
    }

}
