import QtQuick 2.12
import "../../Controls"
import "../../Theme"

Item {
    width: scrollview.width
    height: needHideSection ? 34 : 230

    property string hoveredDescription: ""

    ExpandableHeader {
        id: statisticsHeader
        anchors.top: parent.top
        anchors.topMargin: 4
        headerTitle: title
        headerHided: needHideSection
        isFirst: firstSection
        isLast: lastSection
        onHeaderPressed: {
            myAnilibriaViewModel.myList.toggleSectionHideMark(currentIndex)
        }
        onMoveSection: {
            myAnilibriaViewModel.moveSection(direction, currentIndex);
        }
    }

    Item {
        id: statisticsContent
        anchors.top: statisticsHeader.bottom
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 10
        width: parent.width
        height: needHideSection ? 0 : 200
        visible: !needHideSection

        Item {
            id: middleSeparator
            width: 1
            height: parent.height
            x: parent.width / 2
            y: 0
        }

        PlainText {
            id: watchUsingApplicationText
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 6
            anchors.topMargin: 6
            fontPointSize: 11
            text: "Общее время в приложении: "
        }

        PlainText {
            id: watchUsingApplicationTextValue
            anchors.top: parent.top
            anchors.right: middleSeparator.right
            anchors.topMargin: 6
            fontPointSize: 11
            text: userActivityViewModel.watchUsingApplication
        }

        PlainText {
            id: watchDurationText
            anchors.top: watchUsingApplicationText.bottom
            anchors.left: parent.left
            anchors.leftMargin: 6
            anchors.topMargin: 6
            fontPointSize: 11
            text: "Общее время просмотра: "
        }

        PlainText {
            id: watchDurationTextValue
            anchors.top: watchUsingApplicationText.bottom
            anchors.right: middleSeparator.right
            anchors.leftMargin: 6
            anchors.topMargin: 6
            fontPointSize: 11
            text: userActivityViewModel.watchDuration
        }

        PlainText {
            id: countOpenedReleaseCardText
            anchors.top: watchDurationText.bottom
            anchors.left: parent.left
            anchors.leftMargin: 6
            anchors.topMargin: 6
            fontPointSize: 11
            text: "Сколько раз открывалась карточка релиза: "
        }

        PlainText {
            id: countOpenedReleaseCardTextValue
            anchors.top: watchDurationText.bottom
            anchors.right: middleSeparator.right
            anchors.leftMargin: 6
            anchors.topMargin: 6
            fontPointSize: 11
            text: userActivityViewModel.countOpenedReleaseCard
        }

        PlainText {
            id: countDownloadTorrentText
            anchors.top: countOpenedReleaseCardText.bottom
            anchors.left: parent.left
            anchors.leftMargin: 6
            anchors.topMargin: 6
            fontPointSize: 11
            text: "Сколько раз скачивался торрент: "
        }

        PlainText {
            id: countDownloadTorrentTextValue
            anchors.top: countOpenedReleaseCardText.bottom
            anchors.right: middleSeparator.right
            anchors.leftMargin: 6
            anchors.topMargin: 6
            fontPointSize: 11
            text: userActivityViewModel.countDownloadTorrent
        }

        PlainText {
            id: countAddedToCinemahallText
            anchors.top: countDownloadTorrentText.bottom
            anchors.left: parent.left
            anchors.leftMargin: 6
            anchors.topMargin: 6
            fontPointSize: 11
            text: "Сколько раз релизы доблялись в кинозал: "
        }

        PlainText {
            id: countAddedToCinemahallTextValue
            anchors.top: countDownloadTorrentText.bottom
            anchors.right: middleSeparator.right
            anchors.leftMargin: 6
            anchors.topMargin: 6
            fontPointSize: 11
            text: userActivityViewModel.countAddedToCinemahall
        }

        PlainText {
            id: mostGenresText
            anchors.top: countAddedToCinemahallText.bottom
            anchors.left: parent.left
            anchors.leftMargin: 6
            anchors.topMargin: 6
            fontPointSize: 11
            text: "Наиболее просматриваемые жанры: "
        }

        PlainText {
            id: mostGenresTextValue
            anchors.top: countAddedToCinemahallText.bottom
            anchors.right: middleSeparator.right
            anchors.leftMargin: 6
            anchors.topMargin: 6
            fontPointSize: 11
            text: myAnilibriaViewModel.genres
        }

        PlainText {
            id: mostVoicesText
            anchors.top: mostGenresText.bottom
            anchors.left: parent.left
            anchors.leftMargin: 6
            anchors.topMargin: 6
            fontPointSize: 11
            text: "Наиболее просматриваемые войсеры: "
        }

        PlainText {
            id: mostVoicesTextValue
            anchors.top: mostGenresText.bottom
            anchors.right: middleSeparator.right
            anchors.leftMargin: 6
            anchors.topMargin: 6
            fontPointSize: 11
            text: myAnilibriaViewModel.voices
        }

    }

}
