/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2021 Roman Vladimirov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtWebEngine 1.8

ApplicationWindow {
    id: root
    title: `Комментарии ` + (webViewLoader.item.loading ? `загружаются...` : ``)
    width: 400
    height: 550
    flags: Qt.Dialog

    property bool opened: false

    signal setModalVisible(bool visible)
    signal refreshComments()

    onVisibleChanged: {
        if (visible) showComments();
    }

    onRefreshComments: {
        if (opened) showComments();
    }

    onSetModalVisible: {
        if (visible && !opened) {
            webViewLoader.sourceComponent = webView
            show();
            showComments();
            opened = true;
        }
        if (!visible && opened) {
            hide();
            opened = false;
        }
    }

    onClosing: {
        opened = false;
        webViewLoader.sourceComponent = null;
    }

    function showComments() {
        if (webViewLoader.status === Loader.Ready) webViewLoader.item.url = releasesViewModel.getVkontakteCommentPage(releasesViewModel.openedReleaseCode);
    }

    Loader {
        id: webViewLoader
        anchors.fill: parent
    }

    Component {
        id: webView

        WebEngineView {
            anchors.fill: parent
        }
    }
}
