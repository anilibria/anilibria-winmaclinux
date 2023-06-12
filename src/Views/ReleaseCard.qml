import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import Anilibria.ListModels 1.0
import "../Controls"


ColumnLayout {
    id: cardContainer
    visible: releasesViewModel.isOpenedCard
    anchors.fill: parent
    spacing: 0
    Rectangle {
        color: applicationThemeViewModel.pageBackground
        Layout.fillWidth: true
        Layout.fillHeight: true

        Rectangle {
            width: 42
            height: parent.height
            anchors.right: parent.right
            color: applicationThemeViewModel.pageUpperPanel
        }

        Column {
            id: cardButtons
            width: 42
            anchors.right: parent.right
            IconButton {
                height: 40
                width: 40
                hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                iconPath: applicationThemeViewModel.currentItems.iconReleaseCardCopy
                iconWidth: 26
                iconHeight: 26
                tooltipMessage: "Копировать названия или постер"
                onButtonPressed: {
                    cardCopyMenu.open();
                }

                TextEdit {
                    id: hiddenTextField
                    visible: false
                }

                CommonMenu {
                    id: cardCopyMenu
                    width: 350

                    CommonMenuItem {
                        text: "Копировать название"
                        onPressed: {
                            releasesViewModel.copyToClipboard(releasesViewModel.openedReleaseTitle);
                        }
                    }
                    CommonMenuItem {
                        text: "Копировать оригинальное название"
                        onPressed: {
                            releasesViewModel.copyToClipboard(releasesViewModel.openedReleaseOriginalName);
                        }
                    }
                    CommonMenuItem {
                        text: "Копировать оба названия"
                        onPressed: {
                            releasesViewModel.copyToClipboard(releasesViewModel.openedReleaseTitle + ", " + releasesViewModel.openedReleaseOriginalName);
                        }
                    }
                    CommonMenuItem {
                        text: "Копировать описание"
                        onPressed: {
                            releasesViewModel.copyToClipboard(releasesViewModel.openedReleaseDescription);
                        }
                    }
                    CommonMenuItem {
                        text: "Копировать постер"
                        onPressed: {
                            releasesViewModel.copyImageToClipboard(localStorage.getReleasePosterPath(releasesViewModel.openedReleaseId, releasesViewModel.openedReleasePoster));
                        }
                    }

                }
            }
            IconButton {
                height: 40
                width: 40
                hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                iconPath: applicationThemeViewModel.currentItems.iconSeen
                iconWidth: 26
                iconHeight: 26
                tooltipMessage: "Управление просмотренным и видимостью релиза"
                onButtonPressed: {
                    seenMarkMenu.open();
                }

                CommonMenu {
                    id: seenMarkMenu
                    width: 350

                    CommonMenuItem {
                        text: "Отметить как просмотренное"
                        onPressed: {
                            setSeenStateForOpenedRelease(true);
                        }
                    }
                    CommonMenuItem {
                        text: "Отметить как не просмотренное"
                        onPressed: {
                            setSeenStateForOpenedRelease(false);
                        }
                    }
                    CommonMenuItem {
                        id: hideReleaseCardMenu
                        enabled: releasesViewModel.isOpenedCard && !releasesViewModel.openedReleaseInHided
                        text: "Скрыть релиз"
                        onPressed: {
                            releasesViewModel.addToHidedReleases([releasesViewModel.openedReleaseId]);
                            hideReleaseCardMenu.enabled = false;
                            removeFromHideReleaseCardMenu.enabled = true;
                            seenMarkMenu.close();
                        }
                    }
                    CommonMenuItem {
                        id: removeFromHideReleaseCardMenu
                        enabled: releasesViewModel.isOpenedCard && releasesViewModel.openedReleaseInHided
                        text: "Убрать релиз из скрытых"
                        onPressed: {
                            releasesViewModel.removeFromHidedReleases([releasesViewModel.openedReleaseId]);
                            hideReleaseCardMenu.enabled = true;
                            removeFromHideReleaseCardMenu.enabled = false;
                            seenMarkMenu.close();
                        }
                    }
                }
            }
            IconButton {
                height: 40
                width: 40
                hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                iconPath: applicationThemeViewModel.currentItems.iconFavorites
                iconWidth: 26
                iconHeight: 26
                tooltipMessage: "Добавить или удалить из избранного"
                onButtonPressed: {
                    cardFavoritesMenu.open();
                }

                CommonMenu {
                    id: cardFavoritesMenu
                    width: 350

                    CommonMenuItem {
                        enabled: !releasesViewModel.openedReleaseInFavorites
                        text: "Добавить в избранное"
                        onPressed: {
                            releasesViewModel.addReleaseToFavorites(releasesViewModel.openedReleaseId);
                            cardFavoritesMenu.close();
                        }
                    }
                    CommonMenuItem {
                        enabled: releasesViewModel.openedReleaseInFavorites
                        text: "Удалить из избранного"
                        onPressed: {
                            releasesViewModel.removeReleaseFromFavorites(releasesViewModel.openedReleaseId);
                            cardFavoritesMenu.close();
                        }
                    }
                }
            }
            IconButton {
                height: 40
                width: 40
                hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                iconPath: applicationThemeViewModel.currentItems.iconReleaseCardExternal
                iconWidth: 26
                iconHeight: 26
                tooltipMessage: "Открыть онлайн видео в стороннем плеере"
                onButtonPressed: {
                    externalPlayerMenu.open();
                }

                CommonMenu {
                    id: externalPlayerMenu
                    width: 380

                    CommonMenuItem {
                        text: "Открыть во внешнем плеере в HD качестве"
                        onPressed: {
                            releasesViewModel.openInExternalPlayer(localStorage.packAsM3UAndOpen(releasesViewModel.openedReleaseId, "hd"));
                            externalPlayerMenu.close();
                        }
                    }
                    CommonMenuItem {
                        text: "Открыть во внешнем плеере в SD качестве"
                        onPressed: {
                            releasesViewModel.openInExternalPlayer(localStorage.packAsM3UAndOpen(releasesViewModel.openedReleaseId, "sd"));
                            externalPlayerMenu.close();
                        }
                    }
                    CommonMenuItem {
                        text: "Открыть во внешнем плеере в FullHD качестве"
                        onPressed: {
                            releasesViewModel.openInExternalPlayer(localStorage.packAsM3UAndOpen(releasesViewModel.openedReleaseId, "fullhd"));
                            externalPlayerMenu.close();
                        }
                    }

                    CommonMenuItem {
                        notVisible: Qt.platform.os !== "windows"
                        text: "Открыть в плеере MPC в HD качестве"
                        onPressed: {
                            releasesViewModel.openInExternalPlayer(localStorage.packAsMPCPLAndOpen(releasesViewModel.openedReleaseId, "hd"));
                            externalPlayerMenu.close();
                        }
                    }
                    CommonMenuItem {
                        notVisible: Qt.platform.os !== "windows"
                        text: "Открыть в плеере MPC в SD качестве"
                        onPressed: {
                            releasesViewModel.openInExternalPlayer(localStorage.packAsMPCPLAndOpen(releasesViewModel.openedReleaseId, "sd"));
                            externalPlayerMenu.close();
                        }
                    }
                    CommonMenuItem {
                        notVisible: Qt.platform.os !== "windows"
                        text: "Открыть в плеере MPC в FullHD качестве"
                        onPressed: {
                            releasesViewModel.openInExternalPlayer(localStorage.packAsMPCPLAndOpen(releasesViewModel.openedReleaseId, "fullhd"));
                            externalPlayerMenu.close();
                        }
                    }
                }
            }

            IconButton {
                visible: false
                height: 40
                width: 40
                hoverColor: applicationThemeViewModel.currentItems.filterIconButtonHoverColor
                iconPath: applicationThemeViewModel.currentItems.iconReleaseCatalogSettings
                iconWidth: 26
                iconHeight: 26
                tooltipMessage: "Открыть настройки карточки релиза"
                onButtonPressed: {
                    releaseCardSettingsPopup.open();
                }

                DefaultPopup {
                    id: releaseCardSettingsPopup
                    x: -400
                    width: 400
                    height: 100
                    modal: true
                    focus: true
                    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                    Column {
                        width: parent.width

                    }
                }
            }
        }

        Flickable {
            id: mainScrollArea
            height: parent.height
            width: parent.width - cardButtons.width
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            boundsMovement: Flickable.StopAtBounds
            contentWidth: parent.width - 10
            contentHeight: cardColumn.height
            ScrollBar.vertical: ScrollBar {
                active: true
            }

            Column {
                id: cardColumn

                Grid {
                    id: releaseInfo
                    columnSpacing: 3
                    columns: 3
                    bottomPadding: 4
                    leftPadding: 4
                    topPadding: 4
                    rightPadding: 4

                    CorneredImage {
                        id: cardPoster
                        visible: !mainViewModel.isSmallSizeMode
                        width: 280
                        height: 390
                        posterSource: localStorage.getReleasePosterPath(releasesViewModel.openedReleaseId, releasesViewModel.openedReleasePoster)
                        emptyBorderBackground: applicationThemeViewModel.currentItems.pageBackground

                        MouseArea {
                            anchors.fill: parent
                            onPressed: {
                                releasePosterPreview.isVisible = true;
                            }
                        }
                    }

                    Column {
                        id: descriptionColumn
                        width: page.width - cardButtons.width - (!mainViewModel.isSmallSizeMode ? cardPoster.width : 0)
                        AccentText {
                            textFormat: Text.RichText
                            fontPointSize: 14
                            width: parent.width
                            leftPadding: 8
                            topPadding: 6
                            wrapMode: Text.WordWrap
                            maximumLineCount: 3
                            text: releasesViewModel.openedReleaseTitle
                        }
                        PlainText {
                            textFormat: Text.RichText
                            fontPointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            wrapMode: Text.WordWrap
                            width: parent.width
                            maximumLineCount: 2
                            text: releasesViewModel.openedReleaseOriginalName
                        }
                        AccentText {
                            leftPadding: 8
                            topPadding: 4
                            height: releasesViewModel.openedReleaseAnnounce ? 20 : 0
                            fontPointSize: 10
                            wrapMode: Text.NoWrap
                            elide: Text.ElideRight
                            maximumLineCount: 1
                            text: releasesViewModel.openedReleaseAnnounce
                        }
                        PlainText {
                            fontPointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            text: releasesViewModel.openedReleaseStatusDisplay
                            onLinkActivated: {
                                statusesSearchField.text = releasesViewModel.openedReleaseStatus;
                                releasesViewModel.closeReleaseCard();
                                releasesViewModel.items.refresh();
                            }

                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.NoButton
                                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                            }
                        }
                        PlainText {
                            fontPointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            text: releasesViewModel.openedReleaseYearDisplay
                            onLinkActivated: {
                                yearsSearchField.text = releasesViewModel.openedReleaseYear;
                                releasesViewModel.closeReleaseCard();
                                releasesViewModel.items.refresh();
                            }

                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.NoButton
                                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                            }
                        }
                        PlainText {
                            visible: releasesViewModel.openedReleaseInSchedule
                            fontPointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            text: releasesViewModel.openedReleaseInScheduleDisplay
                        }
                        PlainText {
                            fontPointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            text: releasesViewModel.openedReleaseSeasonDisplay
                            onLinkActivated: {
                                seasonesSearchField.text = releasesViewModel.openedReleaseSeason;
                                releasesViewModel.closeReleaseCard();
                                releasesViewModel.items.refresh();
                            }

                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.NoButton
                                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                            }
                        }
                        PlainText {
                            textFormat: Text.RichText
                            fontPointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            width: parent.width
                            wrapMode: Text.WordWrap
                            maximumLineCount: 2
                            text: releasesViewModel.openedReleaseTypeDisplay
                        }
                        PlainText {
                            fontPointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            width: parent.width
                            wrapMode: Text.WordWrap
                            maximumLineCount: 2
                            text: releasesViewModel.openedReleaseGenresDisplay
                            onLinkActivated: {
                                if (genresSearchField.text.length) {
                                    genresSearchField.text += ", " + link;
                                } else {
                                    genresSearchField.text = link;
                                }
                                releasesViewModel.closeReleaseCard();
                                releasesViewModel.items.refresh();
                            }

                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.NoButton
                                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                            }
                        }
                        PlainText {
                            fontPointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            width: parent.width
                            wrapMode: Text.WordWrap
                            maximumLineCount: 2
                            text: releasesViewModel.openedReleaseVoicesDisplay
                            onLinkActivated: {
                                if (voicesSearchField.text.length) {
                                    voicesSearchField.text += ", " + link;
                                } else {
                                    voicesSearchField.text = link;
                                }
                                releasesViewModel.closeReleaseCard();
                                releasesViewModel.items.refresh();
                            }

                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.NoButton
                                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                            }
                        }

                        Item {
                            width: 1
                            height: 10
                        }

                        Item {
                            width: parent.width
                            height: 40

                            RoundedActionButton {
                                id: watchButton
                                text: qsTr("Смотреть онлайн")
                                textSize: mainViewModel.isSmallSizeMode ? 10 : 11
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 4
                                onClicked: {
                                    watchSingleRelease(releasesViewModel.openedReleaseId, releasesViewModel.openedReleaseVideos, -1, releasesViewModel.openedReleasePoster)

                                    releasePosterPreview.isVisible = false;
                                }
                            }

                            PlainText {
                                id: onlineLabel
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: watchButton.right
                                fontPointSize: 11
                                text: (!mainViewModel.isSmallSizeMode ? "Доступно " : "" ) + releasesViewModel.openedReleaseCountVideos + " серий"
                            }
                        }

                        Item {
                            width: parent.width
                            height: 40

                            PlainText {
                                id: torrentLabel
                                visible: !mainViewModel.isSmallSizeMode
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: watchTorrentButton.right
                                fontPointSize: 11
                                text: "Доступно " + releasesViewModel.openedReleaseCountTorrents + " торрентов"
                            }

                            RoundedActionButton {
                                id: downloadTorrentButton
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 4
                                text: qsTr("Скачать")
                                textSize: mainViewModel.isSmallSizeMode ? 10 : 11
                                onClicked: {
                                    dowloadTorrent.open();
                                }

                                CommonMenu {
                                    id: dowloadTorrent
                                    y: parent.height - parent.height
                                    width: 380

                                    Repeater {
                                        model: releasesViewModel.openedCardTorrents
                                        CommonMenuItem {
                                            text: "Скачать " + quality + " [" + series + "] " + size
                                            onPressed: {
                                                const torrentUri = synchronizationService.combineWithWebSiteUrl(url);
                                                if (localStorage.isUseTorrentStreamMode()) {
                                                    releasesViewModel.downloadTorrent(releasesViewModel.openedReleaseId, torrentUri, userConfigurationViewModel.playerBuffer);
                                                } else {
                                                    synchronizationService.downloadTorrent(torrentUri);
                                                    userActivityViewModel.addDownloadedTorrentToCounter();

                                                    if (userConfigurationViewModel.markAsReadAfterDownload) setSeenStateForOpenedRelease(true);
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            PlainText {
                                id: torrentOrLabel
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: downloadTorrentButton.right
                                fontPointSize: 11
                                text: "или"
                            }

                            RoundedActionButton {
                                id: watchTorrentButton
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: torrentOrLabel.right
                                text: mainViewModel.isSmallSizeMode ? "См. торрент" : "Смотреть торрент"
                                textSize: mainViewModel.isSmallSizeMode ? 10 : 11
                                onClicked: {
                                    if (!userConfigurationViewModel.playerBuffer) {
                                        torrentStreamInfo.open();
                                        return;
                                    }

                                    watchTorrent.open();
                                }

                                CommonMenu {
                                    id: watchTorrent
                                    y: parent.height - parent.height
                                    width: 380

                                    Repeater {
                                        model: releasesViewModel.openedCardTorrents
                                        CommonMenuItem {
                                            text: "Смотреть " + quality + " [" + series + "]"
                                            onPressed: {
                                                watchTorrent.close();

                                                // TODO: if already downloaded torrent start watch local file

                                                if (!onlinePlayerWindowViewModel.isHasVlc) {
                                                    vlcInfo.open();
                                                    return;
                                                }

                                                if (!onlinePlayerWindowViewModel.isSelectedVlc) {
                                                    onlinePlayerWindowViewModel.changePlayer("VLC");
                                                }

                                                onlinePlayerViewModel.quickSetupForSingleTorrentRelease(releasesViewModel.openedReleaseId, identifier, userConfigurationViewModel.playerBuffer);

                                                releasePosterPreview.isVisible = false;

                                                mainViewModel.selectPage("videoplayer");
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        Item {
                            width: parent.width
                            height: 30

                            PlainText {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 14
                                fontPointSize: 11
                                text: releasesViewModel.openedReleaseIsAllSeen ? "Все серии просмотрены" : "Просмотрено " + releasesViewModel.openedReleaseSeenCountVideos + " серий"
                            }
                        }

                        Item {
                            width: parent.width
                            height: 40

                            RoundedActionButton {
                                id: openCommentsButton
                                textSize: mainViewModel.isSmallSizeMode ? 10 : 11
                                text: mainViewModel.isSmallSizeMode ? "Комментарии" : "Открыть комментарии"
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 4
                                onClicked: {
                                    const url = releasesViewModel.getVkontakteCommentPage(releasesViewModel.openedReleaseCode);
                                    Qt.openUrlExternally(url);
                                }

                                property bool isNeedSmallMode: mainViewModel.isSmallSizeMode

                                onIsNeedSmallModeChanged: {
                                    openCommentsButton.state = mainViewModel.isSmallSizeMode ? "smallsizemode" : "";
                                }
                            }
                        }
                    }
                }

                PlainText {
                    id: descriptionText
                    width: mainScrollArea.width - 10
                    fontPointSize: 10
                    leftPadding: 8
                    topPadding: 4
                    wrapMode: Text.WordWrap
                    text: "<b>Описание:</b> " + releasesViewModel.openedReleaseDescription
                    onLinkActivated: {
                        releasesViewModel.openDescriptionLink(link);
                    }

                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.NoButton
                        cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                    }
                }

                Item {
                    width: mainScrollArea.width
                    height: 30

                    PlainText {
                        anchors.centerIn: parent
                        fontPointSize: 11
                        text: "Серии"
                    }
                }

                Item {
                    width: mainScrollArea.width - 10
                    height: 330

                    ReleaseOnlineVideosList {
                        width: parent.width
                        releaseId: releasesViewModel.openedReleaseId
                        onOpenVideo: {
                            watchSingleRelease(releasesViewModel.openedReleaseId, releasesViewModel.openedReleaseVideos, videoId, releasesViewModel.openedReleasePoster);
                        }
                    }
                }

                Item {
                    width: mainScrollArea.width
                    height: 30

                    PlainText {
                        anchors.centerIn: parent
                        fontPointSize: 11
                        text: "Связанные релизы"
                    }
                }

                Item {
                    width: mainScrollArea.width - 10
                    height: 330

                    ReleaseSeriesList {
                        id: releaseSeriesList
                        width: parent.width
                        releaseId: releasesViewModel.openedReleaseId
                        onOpenRelease: {
                            mainViewModel.selectPage("release:" + releaseId);
                        }
                    }
                }

            }
        }
    }

    MessageModal {
        id: torrentStreamInfo
        header: "Приложение TorrentStream не установлено"
        message: "Вы можете изучить инструкцию об установке <a href='https://github.com/anilibria/anilibria-winmaclinux/blob/master/torrentstream.md'>тут</a><br>
            Также если Вы знаете как установить то можете просто скачать последнюю версию <a href='https://github.com/trueromanus/TorrentStream/releases/latest'>отсюда</a><br>"
        content: Row {
            spacing: 6
            anchors.right: parent.right

            RoundedActionButton {
                text: "Закрыть"
                width: 100
                onClicked: {
                    torrentStreamInfo.close();
                }
            }
        }
    }

    MessageModal {
        id: vlcInfo
        header: "Плеер VLC не доступен"
        message: "Ваша версия собрана без плеера VLC. К сожалению только плеер VLC умеет стримить торренты. Для возможности смотреть торрент необходимо собрать приложение с плеером VLC."
        content: Row {
            spacing: 6
            anchors.right: parent.right

            RoundedActionButton {
                text: "Закрыть"
                width: 100
                onClicked: {
                    vlcInfo.close();
                }
            }
        }
    }

}
