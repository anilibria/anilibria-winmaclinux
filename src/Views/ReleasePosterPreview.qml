/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2020 Roman Vladimirov

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
import QtQuick.Layouts 1.3
import "../Controls"
import "../Theme"

Item {
    id: root
    visible: root.isVisible
    enabled: root.isVisible
    anchors.fill: parent
    property bool isVisible: false

    MouseArea {
        visible: root.isVisible
        enabled: root.isVisible
        anchors.fill: parent
        onPressed: {
            root.isVisible = false;
        }
    }

    Rectangle {
        visible: root.isVisible
        enabled: root.isVisible
        color: "black"
        anchors.fill: parent
        opacity: .5
    }

    Image {
        id: previewReleasePoster
        visible: root.isVisible
        enabled: root.isVisible
        anchors.centerIn: parent
        source: releasesViewModel.isOpenedCard ? localStorage.getReleasePosterPath(releasesViewModel.openedReleaseId, releasesViewModel.openedReleasePoster) : ""
        width: 350
        height: 500
    }
}
