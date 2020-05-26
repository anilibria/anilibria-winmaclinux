import QtQuick 2.12
import QtQuick.Controls 2.12
import Anilibria.Services 1.0
import "../Theme"

Page {
    id: root
    background: Rectangle {
        color: ApplicationTheme.pageBackground
    }

    signal navigateFrom()
    signal navigateTo()

    DownloadManager {
        id: downloadManager
        url: "https://de8.libria.fun/get/Xuz5_IQ_kE5ciI4bc0_-Qw/1590520419/mp4/8644/0007.mp4?download=Sword Art Online II-7-hd.mp4"
        destination: ""
    }

    Rectangle {
        anchors.fill: parent

        Text {
            id: label
            text: downloadManager.displayBytesInSeconds
        }

        Button {
            anchors.top: label.bottom
            text: "fdsfadsfasdfasdfasdf"
            onPressed: {
                downloadManager.start();
            }
        }

    }

}
