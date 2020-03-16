import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0
import "../Controls"

Page {
    signal navigateFrom()
    signal navigateTo()

    background: Rectangle {
        color: "#D3D3D3"
    }

    anchors.fill: parent

    RowLayout {
        id: panelContainer
        anchors.fill: parent
        spacing: 0
        Rectangle {
            color: "#9e2323"
            width: 40
            Layout.fillHeight: true
            Column {
                AppPanelButton {
                    iconSource: "../Assets/Icons/menu.svg"
                    width: panelContainer.width
                    onPressed: {
                        drawer.open();
                    }
                }
            }
        }
        Rectangle {
            color: "#D3D3D3"
            Layout.fillHeight: true
            Layout.fillWidth: true

            Rectangle {
                id: cardContainer
                radius: 8
                anchors.centerIn: parent
                width: 600
                height: 460
                ColumnLayout {
                    width: cardContainer.width - 10
                    height: cardContainer.height - 20
                    anchors.horizontalCenter: cardContainer.horizontalCenter
                    anchors.verticalCenter: cardContainer.verticalCenter                    
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        textFormat: Text.RichText
                        text:
                            "<table border='1' cellspacing='1' cellpadding='0'>"+
                            "<tr><td>QIWI</td><td>79660956323</td></tr>" +
                            "<tr><td>Яндекс деньги</td><td>41001990134497</td></tr>" +
                            "<tr><td>Webmoney</td><td>R211016581718, Z720752385996</td></tr>" +
                            "<tr><td>Bitcoin</td><td><a href='https://www.blockchain.com/btc/address/3CarFNZickTNb1nx2Bgk6VammB8CYCBSJd'>3CarFNZickTNb1nx2Bgk6VammB8CYCBSJd</a></td></tr>" +
                            "</table>" +
                            "<p><a href='https://www.patreon.com/anilibria'>https://www.patreon.com/anilibria</a> - ежемесячный платёж!</p>"+
                            "<span>Самый лучший способ поддержать проект.</span>"
                        onLinkActivated: Qt.openUrlExternally(link)
                    }
                }
            }
        }
    }

}
