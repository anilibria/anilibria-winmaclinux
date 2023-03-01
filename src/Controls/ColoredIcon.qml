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
import QtGraphicalEffects 1.0

Item {
    property real iconWidth
    property real iconHeight
    property alias iconSource: _iconImage.source

    width: iconWidth
    height: iconHeight

    Image {
        id: _iconImage
        width: iconWidth
        height: iconHeight
        mipmap: true
    }

    ColorOverlay {
        id: _colorOverlay
        anchors.fill: _iconImage
        source: _iconImage
    }
}

