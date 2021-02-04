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

pragma Singleton
import QtQuick 2.12

Item {
    property bool isDarkTheme: false
    readonly property color plainTextColor: isDarkTheme ? "white" : "black"
    readonly property color headerTextColor: isDarkTheme ? "white" : "#a32727"
    readonly property color linkTextColor: isDarkTheme ? "white" : "#b32121"
    readonly property color pageVerticalPanel: isDarkTheme ? "#262626" : "#9e2323"
    readonly property color pageBackground: isDarkTheme ? "#1f1f1f" : "#D3D3D3"
    readonly property color pageUpperPanel: isDarkTheme ? "#3d3d3d" : "#e6e6e6"
    readonly property color panelBackground: isDarkTheme ? "#474747" : "#f2f2f2"
    readonly property color panelBackgroundShadow: isDarkTheme ? "#78595959" : "#78808080"
    readonly property color roundedButtonBackground: isDarkTheme ? "white" : "#b32121"
    readonly property color roundedButtonForeground: isDarkTheme ? "black" : "white"
    readonly property color roundedButtonHovered: isDarkTheme ? "lightgray" : "#881919"
    readonly property color drawerGradiendStep0: isDarkTheme ? "#1a1a1a" : "#6b0200"
    readonly property color drawerGradiendStep1: isDarkTheme ? "#1f1f1f" : "#ab0909"
    readonly property color drawerGradiendStep2: isDarkTheme ? "#2e2e2e" : "#d41515"
    readonly property color drawerGradiendStep3: isDarkTheme ? "#404040" : "#fc2626"
    readonly property color drawerGradiendStep4: isDarkTheme ? "#454341" : "#f52c39"
    readonly property color filterIconButtonColor: isDarkTheme ? "white" : "black"
    readonly property color filterIconButtonGreenColor: isDarkTheme ? "green" : "green"
    readonly property color filterIconButtonHoverColor: isDarkTheme ? "#737373" : "white"
    readonly property color selectedItem: isDarkTheme ? "white" : "red"
    readonly property color selectedFavorite: isDarkTheme ? "#b30000" : "red"
    readonly property color playerControlBackground: isDarkTheme ? "#82000000" : "#82ffffff"
    readonly property color notificationCenterBackground: isDarkTheme ? "#1a1a1a" : "#e6e6e6"
    readonly property color notificationCenterPanelBackground: isDarkTheme ? "#1a1a1a" : "white"
    readonly property color notificationCenterItemBackground: isDarkTheme ? "#1a1a1a" : "white"
    readonly property color playlistSelectedBackground: isDarkTheme ? "#C8ffffff" : "#64c25656"
    readonly property color playlistBackground: isDarkTheme ? "#C8000000" : "#C8ffffff"
    readonly property color playlistSelectedText: isDarkTheme ? "black" : "white"
    readonly property color playlistText: isDarkTheme ? "white" : "black"
}
