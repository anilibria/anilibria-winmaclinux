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

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QMapIterator>
#include "applicationthemeviewmodel.h"
#include "../../globalhelpers.h"
#include "../../globalconstants.h"

ApplicationThemeViewModel::ApplicationThemeViewModel(QObject *parent)
    : QObject{parent}
{
    auto lightTheme = new QMap<QString, QString>();
    lightTheme->insert(plainTextColorField, "black");
    lightTheme->insert(headerTextColorField, "#a32727");
    lightTheme->insert(linkTextColorField, "#b32121");
    lightTheme->insert(pageVerticalPanelField, "#9e2323");
    lightTheme->insert(pageBackgroundField, "#D3D3D3");
    lightTheme->insert(pageUpperPanelField, "#e6e6e6");
    lightTheme->insert(panelBackgroundField, "#f2f2f2");
    lightTheme->insert(panelBackgroundShadowField, "#78808080");
    lightTheme->insert(roundedButtonBackgroundField, "#b32121");
    lightTheme->insert(roundedButtonBackgroundDisabledField, "#eb9393");
    lightTheme->insert(roundedButtonForegroundField, "white");
    lightTheme->insert(roundedButtonHoveredField, "#881919");
    lightTheme->insert(drawerGradiendStep0Field, "#920710");
    lightTheme->insert(drawerGradiendStep1Field, "#aa0813");
    lightTheme->insert(drawerGradiendStep2Field, "#c30916");
    lightTheme->insert(drawerGradiendStep3Field, "#aa0813");
    lightTheme->insert(drawerGradiendStep4Field, "#920710");
    lightTheme->insert(filterIconButtonColorField, "black");
    lightTheme->insert(filterIconButtonGreenColorField, "green");
    lightTheme->insert(filterIconButtonHoverColorField, "white");
    lightTheme->insert(selectedItemField, "red");
    lightTheme->insert(selectedFavoriteField, "red");
    lightTheme->insert(playerControlBackgroundField, "#82ffffff");
    lightTheme->insert(notificationCenterBackgroundField, "#e6e6e6");
    lightTheme->insert(notificationCenterPanelBackgroundField, "white");
    lightTheme->insert(notificationCenterItemBackgroundField, "white");
    lightTheme->insert(playlistSelectedBackgroundField, "#64c25656");
    lightTheme->insert(playlistBackgroundField, "#C8ffffff");
    lightTheme->insert(playlistSelectedTextField, "white");
    lightTheme->insert(playlistTextField, "black");
    lightTheme->insert(toggleButtonHoveredField, "gray");
    lightTheme->insert(toggleButtonCheckedField, "#9e2323");
    lightTheme->insert(toggleButtonNotCheckedField, "lightgray");
    lightTheme->insert(posterBorderField, "#adadad");
    lightTheme->insert(roundedButtonDisableForegroundField, "white");
    lightTheme->insert(materialAccentField, "#f44336");
    lightTheme->insert(colorBorderInPanelField, "white");
    lightTheme->insert(colorBackgroundNavigationButtonField, "white");
    lightTheme->insert(colorMaterialTextField, "black");
    lightTheme->insert(colorPageIndexTextField, "white");
    lightTheme->insert(colorTooltipBackgroundField, "#616161");
    lightTheme->insert(colorDrawerItemTextField, "white");
    lightTheme->insert(colorDrawerItemHoveredField, "#50000000");
    lightTheme->insert(colorPosterFilterField, "transparent");
    lightTheme->insert(colorPopupBackgroundField, "#ffffff");
    lightTheme->insert(colorPopupBackgroundShadowField, "#99303030");
    lightTheme->insert(colorSwitchHighlightedRippleField, "#1ef44336");
    lightTheme->insert(colorSwitchRippleField, "#1ef44336");
    lightTheme->insert(colorSwitchCheckedTrackField, "#80f44336");
    lightTheme->insert(colorSwitchUncheckedTrackField, "#42000000");
    lightTheme->insert(colorSwitchDisabledTrackField, "#1e000000");
    lightTheme->insert(colorSwitchCheckedHandleField, "#f44336");
    lightTheme->insert(colorSwitchUncheckedHandleField, "#fafafa");
    lightTheme->insert(colorSwitchDisabledHandleField, "#bdbdbd");

    lightTheme->insert(colorTextFieldAccentField, "#f44336");
    lightTheme->insert(colorTextFieldForegroundField, "#000000");
    lightTheme->insert(colorTextFieldHintField, "#60000000");
    lightTheme->insert(colorTextFieldHighlightField, "#dd000000");
    lightTheme->insert(colorTextFieldTextField, "#dd000000");

    lightTheme->insert(iconMainMenuField, "qrc:/Assets/Icons/coloreddrawer.svg");
    lightTheme->insert(iconMainMenuReleasesField, "qrc:/Assets/Icons/catalogmenu.svg");
    lightTheme->insert(iconMainMenuCinemahallField, "qrc:/Assets/Icons/cinemahallmenu.svg");
    lightTheme->insert(iconMainMenuReleasesSeriesField, "qrc:/Assets/Icons/seriesmenu.svg");
    lightTheme->insert(iconMainMenuMaintenanceField, "qrc:/Assets/Icons/maintenance.svg");
    lightTheme->insert(iconMainMenuMyAnilibriaField, "qrc:/Assets/Icons/house.svg");
    lightTheme->insert(iconMainMenuVideoplayerField, "qrc:/Assets/Icons/videoplayermenu.svg");
    lightTheme->insert(iconMainMenuDonateField, "qrc:/Assets/Icons/donate.svg");
    lightTheme->insert(iconMainMenuThemeManagerField, "qrc:/Assets/Icons/themes.svg");
    lightTheme->insert(iconMainMenuYoutubeField, "qrc:/Assets/Icons/youtube.svg");
    lightTheme->insert(iconMainMenuTorrentStreamField, "qrc:/Assets/Icons/utorrent.svg");
    lightTheme->insert(iconMainMenuAuthorizationField, "qrc:/Assets/Icons/user.svg");
    lightTheme->insert(iconLeftHalfScreenField, "qrc:/Assets/Icons/coloredleftmenu.svg");
    lightTheme->insert(iconRightHalfScreenField, "qrc:/Assets/Icons/coloredrightmenu.svg");
    lightTheme->insert(iconMinimizeWindowField, "qrc:/Assets/Icons/grayedleftmenu.svg");
    lightTheme->insert(iconMaximizeWindowField, "qrc:/Assets/Icons/grayedrightmenu.svg");
    lightTheme->insert(iconNormalWindowField, "qrc:/Assets/Icons/gofromfullscreen.svg");
    lightTheme->insert(iconCloseWindowField, "qrc:/Assets/Icons/coloredclosewindow.svg");
    lightTheme->insert(iconFavoritesField, "qrc:/Assets/Icons/ratingcolor.svg");
    lightTheme->insert(iconSeenField, "qrc:/Assets/Icons/coloredeye.svg");
    lightTheme->insert(iconInfoField, "qrc:/Assets/Icons/information.svg");
    lightTheme->insert(iconClearTextControlField, "qrc:/Assets/Icons/closewindow.svg");
    lightTheme->insert(iconBackToTopField, "qrc:/Assets/Icons/arrowup.svg");
    lightTheme->insert(iconReleaseCatalogSynchronizationField, "qrc:/Assets/Icons/coloredrefresh.svg");
    lightTheme->insert(iconReleaseCatalogSearchField, "qrc:/Assets/Icons/search.svg");
    lightTheme->insert(iconReleaseCatalogNotificationField, "qrc:/Assets/Icons/coloredbell.svg");
    lightTheme->insert(iconReleaseCatalogRandomField, "qrc:/Assets/Icons/coloreddice.svg");
    lightTheme->insert(iconReleaseCatalogSettingsField, "qrc:/Assets/Icons/coloredsettings.svg");
    lightTheme->insert(iconReleaseCatalogHidedButtonsField, "qrc:/Assets/Icons/hidebuttonmenu.svg");
    lightTheme->insert(iconReleaseCatalogOnlineField, "qrc:/Assets/Icons/online.svg");
    lightTheme->insert(iconReleaseCatalogTorrentField, "qrc:/Assets/Icons/utorrent.svg");
    lightTheme->insert(iconReleaseCatalogScheduleField, "qrc:/Assets/Icons/calendarcolor.svg");
    lightTheme->insert(iconReleaseCatalogHistoryField, "qrc:/Assets/Icons/history.svg");
    lightTheme->insert(iconReleaseCatalogAlphabetField, "qrc:/Assets/Icons/alphabet.svg");
    lightTheme->insert(iconReleaseCatalogAllReleasesField, "qrc:/Assets/Icons/allreleases.svg");
    lightTheme->insert(iconReleaseCatalogCompilationField, "qrc:/Assets/Icons/listcustom.svg");
    lightTheme->insert(iconReleaseCatalogSortAscField, "qrc:/Assets/Icons/sortascending.svg");
    lightTheme->insert(iconReleaseCatalogSortDescField, "qrc:/Assets/Icons/sortdescending.svg");
    lightTheme->insert(iconReleaseCardListMenuField, "qrc:/Assets/Icons/allreleases.svg");
    lightTheme->insert(iconReleaseCardCloseField, "qrc:/Assets/Icons/coloredclosewindow.svg");
    lightTheme->insert(iconReleaseCardCopyField, "qrc:/Assets/Icons/copy.svg");
    lightTheme->insert(iconReleaseCardExternalField, "qrc:/Assets/Icons/external.svg");
    lightTheme->insert(iconNavigationButtonLeftField, "qrc:/Assets/Icons/leftscroll.svg");
    lightTheme->insert(iconNavigationButtonRightField, "qrc:/Assets/Icons/rightscroll.svg");
    lightTheme->insert(iconMyAnilibriaButtonSortUpField, "qrc:/Assets/Icons/coloredsortup.svg");
    lightTheme->insert(iconMyAnilibriaButtonSortBottomField, "qrc:/Assets/Icons/coloredsortdown.svg");
    lightTheme->insert(iconMyAnilibriaButtonPlusField, "qrc:/Assets/Icons/plus.svg");
    lightTheme->insert(iconMyAnilibriaButtonMinusField, "qrc:/Assets/Icons/minus.svg");
    lightTheme->insert(iconMyAnilibriaButtonSortSectionsField, "qrc:/Assets/Icons/videos.svg");
    lightTheme->insert(iconDeleteItemField, "qrc:/Assets/Icons/delete.svg");
    lightTheme->insert(iconDownloadThemeField, "qrc:/Assets/Icons/downloadtheme.svg");
    lightTheme->insert(iconEditThemeFieldPlusField, "qrc:/Assets/Icons/plus.svg");
    lightTheme->insert(iconEmptyField, "qrc:/Assets/Icons/emptybox.svg");
    lightTheme->insert(iconPlayerFullscreenField, "qrc:/Assets/Icons/videoplayer/fullscreen.svg");
    lightTheme->insert(iconPlayerCropModeField, "qrc:/Assets/Icons/videoplayer/resize.svg");
    lightTheme->insert(iconPlayerSettingsField, "qrc:/Assets/Icons/videoplayer/settings.svg");
    lightTheme->insert(iconPlayerRemoteField, "qrc:/Assets/Icons/videoplayer/network.svg");
    lightTheme->insert(iconPlayerTopMostField, "qrc:/Assets/Icons/videoplayer/topmost.svg");
    lightTheme->insert(iconPlayerRefreshField, "qrc:/Assets/Icons/videoplayer/reload.svg");
    lightTheme->insert(iconPlayerInfoField, "qrc:/Assets/Icons/videoplayer/info.svg");
    lightTheme->insert(iconPlayerMuteField, "qrc:/Assets/Icons/videoplayer/muteon.svg");
    lightTheme->insert(iconPlayerUnMuteField, "qrc:/Assets/Icons/videoplayer/muteoff.svg");
    lightTheme->insert(iconPlayerSeenField, "qrc:/Assets/Icons/videoplayer/seenmarkselected.svg");
    lightTheme->insert(iconPlayerUnseenField, "qrc:/Assets/Icons/videoplayer/seenmark.svg");
    lightTheme->insert(iconPlayerExternalField, "qrc:/Assets/Icons/videoplayer/externalplayer.svg");
    lightTheme->insert(iconPlayerPlayField, "qrc:/Assets/Icons/videoplayer/play.svg");
    lightTheme->insert(iconPlayerPauseField, "qrc:/Assets/Icons/videoplayer/pause.svg");
    lightTheme->insert(iconPlayerJumpLeftField, "qrc:/Assets/Icons/videoplayer/leftjump.svg");
    lightTheme->insert(iconPlayerJumpRightField, "qrc:/Assets/Icons/videoplayer/rightjump.svg");
    lightTheme->insert(iconPlayerNextSeriaField, "qrc:/Assets/Icons/videoplayer/nextseria.svg");
    lightTheme->insert(iconPlayerPreviousSeriaField, "qrc:/Assets/Icons/videoplayer/previousseria.svg");
    lightTheme->insert(iconPlayerDrawerField, "qrc:/Assets/Icons/videoplayer/menu.svg");
    lightTheme->insert(iconNotificationField, "qrc:/Assets/Icons/notification/notification.svg");
    lightTheme->insert(iconNotificationSelectedField, "qrc:/Assets/Icons/notification/notificationselected.svg");    

    lightTheme->insert(colorTooltipTextField, "white");
    lightTheme->insert(colorSeenReleaseItemTextField, "green");

    lightTheme->insert(basedOnThemeField, m_lightTheme);
    lightTheme->insert(externalIdField, "");
    m_themes.insert(m_lightTheme, lightTheme);

    auto darkTheme = new QMap<QString, QString>();
    darkTheme->insert(plainTextColorField, "white");
    darkTheme->insert(headerTextColorField, "white");
    darkTheme->insert(linkTextColorField, "white");
    darkTheme->insert(pageVerticalPanelField, "#262626");
    darkTheme->insert(pageBackgroundField, "#1f1f1f");
    darkTheme->insert(pageUpperPanelField, "#3d3d3d");
    darkTheme->insert(panelBackgroundField, "#474747");
    darkTheme->insert(panelBackgroundShadowField, "#78595959");
    darkTheme->insert(roundedButtonBackgroundField, "white");
    darkTheme->insert(roundedButtonBackgroundDisabledField, "#f2f2f2");
    darkTheme->insert(roundedButtonForegroundField, "black");
    darkTheme->insert(roundedButtonHoveredField, "lightgray");
    darkTheme->insert(drawerGradiendStep0Field, "#1a1a1a");
    darkTheme->insert(drawerGradiendStep1Field, "#1f1f1f");
    darkTheme->insert(drawerGradiendStep2Field, "#2e2e2e");
    darkTheme->insert(drawerGradiendStep3Field, "#404040");
    darkTheme->insert(drawerGradiendStep4Field, "#454341");
    darkTheme->insert(filterIconButtonColorField, "white");
    darkTheme->insert(filterIconButtonGreenColorField, "green");
    darkTheme->insert(filterIconButtonHoverColorField, "#737373");
    darkTheme->insert(selectedItemField, "white");
    darkTheme->insert(selectedFavoriteField, "#b30000");
    darkTheme->insert(playerControlBackgroundField, "#82000000");
    darkTheme->insert(notificationCenterBackgroundField, "#1a1a1a");
    darkTheme->insert(notificationCenterPanelBackgroundField, "#1a1a1a");
    darkTheme->insert(notificationCenterItemBackgroundField, "#1a1a1a");
    darkTheme->insert(playlistSelectedBackgroundField, "#C8ffffff");
    darkTheme->insert(playlistBackgroundField, "#C8000000");
    darkTheme->insert(playlistSelectedTextField, "black");
    darkTheme->insert(playlistTextField, "white");
    darkTheme->insert(toggleButtonHoveredField, "gray");
    darkTheme->insert(toggleButtonCheckedField, "#9e2323");
    darkTheme->insert(toggleButtonNotCheckedField, "darkgrey");
    darkTheme->insert(posterBorderField, "#adadad");
    darkTheme->insert(roundedButtonDisableForegroundField, "lightgray");
    darkTheme->insert(materialAccentField, "#f44336");
    darkTheme->insert(colorBorderInPanelField, "white");
    darkTheme->insert(colorBackgroundNavigationButtonField, "white");
    darkTheme->insert(colorMaterialTextField, "white");
    darkTheme->insert(colorPageIndexTextField, "white");
    darkTheme->insert(colorTooltipBackgroundField, "#616161");
    darkTheme->insert(colorDrawerItemTextField, "white");
    darkTheme->insert(colorDrawerItemHoveredField, "#50ffffff");
    darkTheme->insert(colorPosterFilterField, "#50000000");
    darkTheme->insert(colorPopupBackgroundField, "#424242");
    darkTheme->insert(colorPopupBackgroundShadowField, "#99fafafa");
    darkTheme->insert(colorSwitchHighlightedRippleField, "#32f44336");
    darkTheme->insert(colorSwitchRippleField, "#32f44336");
    darkTheme->insert(colorSwitchCheckedTrackField, "#80f44336");
    darkTheme->insert(colorSwitchUncheckedTrackField, "#4cffffff");
    darkTheme->insert(colorSwitchDisabledTrackField, "#19ffffff");
    darkTheme->insert(colorSwitchCheckedHandleField, "#f44336");
    darkTheme->insert(colorSwitchUncheckedHandleField, "#bdbdbd");
    darkTheme->insert(colorSwitchDisabledHandleField, "#424242");

    darkTheme->insert(colorTextFieldAccentField, "#f44336");
    darkTheme->insert(colorTextFieldForegroundField, "#ffffff");
    darkTheme->insert(colorTextFieldHintField, "#4cffffff");
    darkTheme->insert(colorTextFieldHighlightField, "#ffffff");
    darkTheme->insert(colorTextFieldTextField, "#ffffff");

    darkTheme->insert(iconMainMenuField, "qrc:/Assets/Icons/coloreddrawer.svg");
    darkTheme->insert(iconMainMenuReleasesField, "qrc:/Assets/Icons/catalogmenu.svg");
    darkTheme->insert(iconMainMenuCinemahallField, "qrc:/Assets/Icons/cinemahallmenu.svg");
    darkTheme->insert(iconMainMenuReleasesSeriesField, "qrc:/Assets/Icons/seriesmenu.svg");
    darkTheme->insert(iconMainMenuMaintenanceField, "qrc:/Assets/Icons/maintenance.svg");
    darkTheme->insert(iconMainMenuMyAnilibriaField, "qrc:/Assets/Icons/house.svg");
    darkTheme->insert(iconMainMenuVideoplayerField, "qrc:/Assets/Icons/videoplayermenu.svg");
    darkTheme->insert(iconMainMenuDonateField, "qrc:/Assets/Icons/donate.svg");
    darkTheme->insert(iconMainMenuThemeManagerField, "qrc:/Assets/Icons/themes.svg");
    darkTheme->insert(iconMainMenuYoutubeField, "qrc:/Assets/Icons/youtube.svg");
    darkTheme->insert(iconMainMenuTorrentStreamField, "qrc:/Assets/Icons/utorrent.svg");
    darkTheme->insert(iconMainMenuAuthorizationField, "qrc:/Assets/Icons/user.svg");
    darkTheme->insert(iconLeftHalfScreenField, "qrc:/Assets/Icons/coloredleftmenu.svg");
    darkTheme->insert(iconRightHalfScreenField, "qrc:/Assets/Icons/coloredrightmenu.svg");
    darkTheme->insert(iconMinimizeWindowField, "qrc:/Assets/Icons/grayedleftmenu.svg");
    darkTheme->insert(iconMaximizeWindowField, "qrc:/Assets/Icons/grayedrightmenu.svg");
    darkTheme->insert(iconNormalWindowField, "qrc:/Assets/Icons/gofromfullscreen.svg");
    darkTheme->insert(iconCloseWindowField, "qrc:/Assets/Icons/coloredclosewindow.svg");
    darkTheme->insert(iconFavoritesField, "qrc:/Assets/Icons/ratingcolor.svg");
    darkTheme->insert(iconSeenField, "qrc:/Assets/Icons/coloredeye.svg");
    darkTheme->insert(iconInfoField, "qrc:/Assets/Icons/information.svg");
    darkTheme->insert(iconClearTextControlField, "qrc:/Assets/Icons/closewindowwhite.svg");
    darkTheme->insert(iconBackToTopField, "qrc:/Assets/Icons/darkarrowup.svg");
    darkTheme->insert(iconReleaseCatalogSynchronizationField, "qrc:/Assets/Icons/coloredrefresh.svg");
    darkTheme->insert(iconReleaseCatalogSearchField, "qrc:/Assets/Icons/search.svg");
    darkTheme->insert(iconReleaseCatalogNotificationField, "qrc:/Assets/Icons/coloredbell.svg");
    darkTheme->insert(iconReleaseCatalogRandomField, "qrc:/Assets/Icons/coloreddice.svg");
    darkTheme->insert(iconReleaseCatalogSettingsField, "qrc:/Assets/Icons/coloredsettings.svg");
    darkTheme->insert(iconReleaseCatalogHidedButtonsField, "qrc:/Assets/Icons/hidebuttonmenu.svg");
    darkTheme->insert(iconReleaseCatalogOnlineField, "qrc:/Assets/Icons/online.svg");
    darkTheme->insert(iconReleaseCatalogTorrentField, "qrc:/Assets/Icons/utorrent.svg");
    darkTheme->insert(iconReleaseCatalogScheduleField, "qrc:/Assets/Icons/calendarcolor.svg");
    darkTheme->insert(iconReleaseCatalogHistoryField, "qrc:/Assets/Icons/history.svg");
    darkTheme->insert(iconReleaseCatalogAlphabetField, "qrc:/Assets/Icons/alphabet.svg");
    darkTheme->insert(iconReleaseCatalogAllReleasesField, "qrc:/Assets/Icons/allreleases.svg");
    darkTheme->insert(iconReleaseCatalogCompilationField, "qrc:/Assets/Icons/listcustom.svg");
    darkTheme->insert(iconReleaseCatalogSortAscField, "qrc:/Assets/Icons/darksortascending.svg");
    darkTheme->insert(iconReleaseCatalogSortDescField, "qrc:/Assets/Icons/darksortdescending.svg");
    darkTheme->insert(iconReleaseCardListMenuField, "qrc:/Assets/Icons/allreleases.svg");
    darkTheme->insert(iconReleaseCardCloseField, "qrc:/Assets/Icons/coloredclosewindow.svg");
    darkTheme->insert(iconReleaseCardCopyField, "qrc:/Assets/Icons/copy.svg");
    darkTheme->insert(iconReleaseCardExternalField, "qrc:/Assets/Icons/external.svg");
    darkTheme->insert(iconNavigationButtonLeftField, "qrc:/Assets/Icons/leftscroll.svg");
    darkTheme->insert(iconNavigationButtonRightField, "qrc:/Assets/Icons/rightscroll.svg");
    darkTheme->insert(iconMyAnilibriaButtonSortUpField, "qrc:/Assets/Icons/coloredsortup.svg");
    darkTheme->insert(iconMyAnilibriaButtonSortBottomField, "qrc:/Assets/Icons/coloredsortdown.svg");
    darkTheme->insert(iconMyAnilibriaButtonPlusField, "qrc:/Assets/Icons/plus.svg");
    darkTheme->insert(iconMyAnilibriaButtonMinusField, "qrc:/Assets/Icons/minus.svg");
    darkTheme->insert(iconMyAnilibriaButtonSortSectionsField, "qrc:/Assets/Icons/videos.svg");
    darkTheme->insert(iconDeleteItemField, "qrc:/Assets/Icons/delete.svg");
    darkTheme->insert(iconDownloadThemeField, "qrc:/Assets/Icons/downloadtheme.svg");
    darkTheme->insert(iconEditThemeFieldPlusField, "qrc:/Assets/Icons/plus.svg");
    darkTheme->insert(iconEmptyField, "qrc:/Assets/Icons/emptybox.svg");
    darkTheme->insert(iconPlayerFullscreenField, "qrc:/Assets/Icons/videoplayer/darkfullscreen.svg");
    darkTheme->insert(iconPlayerCropModeField, "qrc:/Assets/Icons/videoplayer/darkresize.svg");
    darkTheme->insert(iconPlayerSettingsField, "qrc:/Assets/Icons/videoplayer/darksettings.svg");
    darkTheme->insert(iconPlayerRemoteField, "qrc:/Assets/Icons/videoplayer/darknetwork.svg");
    darkTheme->insert(iconPlayerTopMostField, "qrc:/Assets/Icons/videoplayer/darktopmost.svg");
    darkTheme->insert(iconPlayerRefreshField, "qrc:/Assets/Icons/videoplayer/darkreload.svg");
    darkTheme->insert(iconPlayerInfoField, "qrc:/Assets/Icons/videoplayer/darkinfo.svg");
    darkTheme->insert(iconPlayerMuteField, "qrc:/Assets/Icons/videoplayer/darkmuteon.svg");
    darkTheme->insert(iconPlayerUnMuteField, "qrc:/Assets/Icons/videoplayer/darkmuteoff.svg");
    darkTheme->insert(iconPlayerSeenField, "qrc:/Assets/Icons/videoplayer/seenmarkselected.svg");
    darkTheme->insert(iconPlayerUnseenField, "qrc:/Assets/Icons/videoplayer/darkseenmark.svg");
    darkTheme->insert(iconPlayerExternalField, "qrc:/Assets/Icons/videoplayer/darkexternalplayer.svg");
    darkTheme->insert(iconPlayerPlayField, "qrc:/Assets/Icons/videoplayer/darkplay.svg");
    darkTheme->insert(iconPlayerPauseField, "qrc:/Assets/Icons/videoplayer/darkpause.svg");
    darkTheme->insert(iconPlayerJumpLeftField, "qrc:/Assets/Icons/videoplayer/darkleftjump.svg");
    darkTheme->insert(iconPlayerJumpRightField, "qrc:/Assets/Icons/videoplayer/darkrightjump.svg");
    darkTheme->insert(iconPlayerNextSeriaField, "qrc:/Assets/Icons/videoplayer/darknextseria.svg");
    darkTheme->insert(iconPlayerPreviousSeriaField, "qrc:/Assets/Icons/videoplayer/darkpreviousseria.svg");
    darkTheme->insert(iconPlayerDrawerField, "qrc:/Assets/Icons/videoplayer/darkmenu.svg");
    darkTheme->insert(iconNotificationField, "qrc:/Assets/Icons/notification/darknotification.svg");
    darkTheme->insert(iconNotificationSelectedField, "qrc:/Assets/Icons/notification/notificationselected.svg");

    darkTheme->insert(colorTooltipTextField, "white");
    darkTheme->insert(colorSeenReleaseItemTextField, "green");

    darkTheme->insert(basedOnThemeField, m_darkTheme);
    darkTheme->insert(externalIdField, "");
    m_themes.insert(m_darkTheme, darkTheme);

    m_cachePathName = getCachePath(m_cachePathName);
    createIfNotExistsFile(m_cachePathName, "{}");

    m_fields.append(plainTextColorField);
    m_fields.append(headerTextColorField);
    m_fields.append(linkTextColorField);
    m_fields.append(pageVerticalPanelField);
    m_fields.append(pageBackgroundField);
    m_fields.append(pageUpperPanelField);
    m_fields.append(panelBackgroundField);
    m_fields.append(panelBackgroundShadowField);
    m_fields.append(roundedButtonBackgroundField);
    m_fields.append(roundedButtonBackgroundDisabledField);
    m_fields.append(roundedButtonHoveredField);
    m_fields.append(roundedButtonForegroundField);
    m_fields.append(roundedButtonDisableForegroundField);
    m_fields.append(drawerGradiendStep0Field);
    m_fields.append(drawerGradiendStep1Field);
    m_fields.append(drawerGradiendStep2Field);
    m_fields.append(drawerGradiendStep3Field);
    m_fields.append(drawerGradiendStep4Field);
    m_fields.append(filterIconButtonColorField);
    m_fields.append(filterIconButtonGreenColorField);
    m_fields.append(filterIconButtonHoverColorField);
    m_fields.append(selectedItemField);
    m_fields.append(selectedFavoriteField);
    m_fields.append(playerControlBackgroundField);
    m_fields.append(notificationCenterBackgroundField);
    m_fields.append(notificationCenterPanelBackgroundField);
    m_fields.append(notificationCenterItemBackgroundField);
    m_fields.append(playlistSelectedBackgroundField);
    m_fields.append(playlistBackgroundField);
    m_fields.append(playlistSelectedTextField);
    m_fields.append(playlistTextField);
    m_fields.append(toggleButtonHoveredField);
    m_fields.append(toggleButtonCheckedField);
    m_fields.append(toggleButtonNotCheckedField);
    m_fields.append(posterBorderField);
    m_fields.append(materialAccentField);
    m_fields.append(colorMaterialTextField);
    m_fields.append(colorBorderInPanelField);
    m_fields.append(colorBackgroundNavigationButtonField);
    m_fields.append(colorPageIndexTextField);
    m_fields.append(colorTooltipTextField);
    m_fields.append(colorSeenReleaseItemTextField);
    m_fields.append(colorTooltipBackgroundField);
    m_fields.append(colorDrawerItemTextField);
    m_fields.append(colorDrawerItemHoveredField);
    m_fields.append(colorPosterFilterField);
    m_fields.append(colorPopupBackgroundField);
    m_fields.append(colorPopupBackgroundShadowField);

    m_fields.append(colorSwitchHighlightedRippleField);
    m_fields.append(colorSwitchRippleField);
    m_fields.append(colorSwitchCheckedTrackField);
    m_fields.append(colorSwitchUncheckedTrackField);
    m_fields.append(colorSwitchDisabledTrackField);
    m_fields.append(colorSwitchCheckedHandleField);
    m_fields.append(colorSwitchUncheckedHandleField);
    m_fields.append(colorSwitchDisabledHandleField);
    m_fields.append(colorTextFieldAccentField);
    m_fields.append(colorTextFieldForegroundField);
    m_fields.append(colorTextFieldHintField);
    m_fields.append(colorTextFieldHighlightField);
    m_fields.append(colorTextFieldTextField);

    m_fields.append(iconMainMenuField);
    m_fields.append(iconMainMenuReleasesField);
    m_fields.append(iconMainMenuCinemahallField);
    m_fields.append(iconMainMenuReleasesSeriesField);
    m_fields.append(iconMainMenuMaintenanceField);
    m_fields.append(iconMainMenuMyAnilibriaField);
    m_fields.append(iconMainMenuVideoplayerField);
    m_fields.append(iconMainMenuDonateField);
    m_fields.append(iconMainMenuThemeManagerField);
    m_fields.append(iconMainMenuYoutubeField);
    m_fields.append(iconMainMenuTorrentStreamField);
    m_fields.append(iconMainMenuAuthorizationField);
    m_fields.append(iconLeftHalfScreenField);
    m_fields.append(iconRightHalfScreenField);
    m_fields.append(iconMinimizeWindowField);
    m_fields.append(iconMaximizeWindowField);
    m_fields.append(iconNormalWindowField);
    m_fields.append(iconCloseWindowField);
    m_fields.append(iconFavoritesField);
    m_fields.append(iconSeenField);
    m_fields.append(iconInfoField);
    m_fields.append(iconClearTextControlField);
    m_fields.append(iconBackToTopField);
    m_fields.append(iconReleaseCatalogSynchronizationField);
    m_fields.append(iconReleaseCatalogSearchField);
    m_fields.append(iconReleaseCatalogNotificationField);
    m_fields.append(iconReleaseCatalogRandomField);
    m_fields.append(iconReleaseCatalogSettingsField);
    m_fields.append(iconReleaseCatalogHidedButtonsField);
    m_fields.append(iconReleaseCatalogOnlineField);
    m_fields.append(iconReleaseCatalogTorrentField);
    m_fields.append(iconReleaseCatalogScheduleField);
    m_fields.append(iconReleaseCatalogHistoryField);
    m_fields.append(iconReleaseCatalogAlphabetField);
    m_fields.append(iconReleaseCatalogAllReleasesField);
    m_fields.append(iconReleaseCatalogCompilationField);
    m_fields.append(iconReleaseCatalogSortAscField);
    m_fields.append(iconReleaseCatalogSortDescField);
    m_fields.append(iconReleaseCardListMenuField);
    m_fields.append(iconReleaseCardCloseField);
    m_fields.append(iconReleaseCardCopyField);
    m_fields.append(iconReleaseCardExternalField);
    m_fields.append(iconNavigationButtonLeftField);
    m_fields.append(iconNavigationButtonRightField);
    m_fields.append(iconMyAnilibriaButtonSortUpField);
    m_fields.append(iconMyAnilibriaButtonSortBottomField);
    m_fields.append(iconMyAnilibriaButtonPlusField);
    m_fields.append(iconMyAnilibriaButtonMinusField);    
    m_fields.append(iconMyAnilibriaButtonSortSectionsField);
    m_fields.append(iconDeleteItemField);
    m_fields.append(iconDownloadThemeField);
    m_fields.append(iconEditThemeFieldPlusField);
    m_fields.append(iconPlayerFullscreenField);
    m_fields.append(iconPlayerCropModeField);
    m_fields.append(iconPlayerSettingsField);
    m_fields.append(iconPlayerRemoteField);
    m_fields.append(iconPlayerTopMostField);
    m_fields.append(iconPlayerRefreshField);
    m_fields.append(iconPlayerInfoField);
    m_fields.append(iconPlayerMuteField);
    m_fields.append(iconPlayerUnMuteField);
    m_fields.append(iconPlayerSeenField);
    m_fields.append(iconPlayerUnseenField);
    m_fields.append(iconPlayerExternalField);
    m_fields.append(iconPlayerPlayField);
    m_fields.append(iconPlayerPauseField);
    m_fields.append(iconPlayerJumpLeftField);
    m_fields.append(iconPlayerJumpRightField);
    m_fields.append(iconPlayerNextSeriaField);
    m_fields.append(iconPlayerPreviousSeriaField);
    m_fields.append(iconPlayerDrawerField);
    m_fields.append(iconNotificationField);
    m_fields.append(iconNotificationSelectedField);
    m_fields.append(iconEmptyField);

    m_fields.append(externalIdField);

    readCacheFile();

    m_menuItems.append("Установленные локально");
    m_menuItems.append("Доступные для скачивания");
    m_menuItems.append("Редактор темы");

    m_fieldList->fillFields(m_fields);
    m_localThemes->setItems(m_localIds);

    connect(m_service, &ThemeManagerService::themesLoaded, this, &ApplicationThemeViewModel::themesLoaded);
    connect(m_service, &ThemeManagerService::themeLoaded, this, &ApplicationThemeViewModel::themeLoaded);

    setCurrentItems();
}

void ApplicationThemeViewModel::setSelectedTheme(const QString &selectedTheme) noexcept
{
    if (m_selectedTheme == selectedTheme) return;

    m_selectedTheme = selectedTheme;
    emit selectedThemeChanged();

    auto currentTheme = m_themes.value(m_selectedTheme);
    m_basedOnDark = currentTheme->value(basedOnThemeField) == m_darkTheme;

    emitAllFields();
    setCurrentItems();

    m_service->loadThemes(m_basedOnDark);
}

void ApplicationThemeViewModel::setSelectedMenuItem(int selectedMenuItem) noexcept
{
    if (m_selectedMenuItem == selectedMenuItem) return;

    m_selectedMenuItem = selectedMenuItem;

    emit selectedMenuItemChanged();
    emit selectedMenuItemNameChanged();
}

QVariantMap ApplicationThemeViewModel::previewItems() const noexcept
{
    QVariantMap map;

    if (m_fieldList == nullptr) return map;

    auto overridedValues = m_fieldList->getValues();

    auto basedThemeName = m_fieldList->basedOnTheme();
    auto basedTheme = m_themes.value(basedThemeName);

    foreach (auto field, m_fields) {
        if (overridedValues.contains(field)) {
            map.insert(field, overridedValues.value(field));
            continue;
        }
        if (basedTheme != nullptr) {
            map.insert(field, basedTheme->value(field));
            continue;
        }
        map.insert(field, "");
    }

    return map;
}

void ApplicationThemeViewModel::setCurrentItems() noexcept
{
    m_currentItems.clear();

    auto theme = m_themes.value(m_selectedTheme);

    foreach (auto field, m_fields) {
        m_currentItems.insert(field, theme->value(field));
    }

    emit currentItemsChanged();
}

void ApplicationThemeViewModel::setNotAddCopyToName(bool notAddCopyToName) noexcept
{
    if (notAddCopyToName == m_notAddCopyToName) return;

    m_notAddCopyToName = notAddCopyToName;
    emit notAddCopyToNameChanged();
}

void ApplicationThemeViewModel::saveCurrentState()
{
    QJsonArray themes;
    QMapIterator<QString, QMap<QString, QString>*> iterator(m_themes);
    while (iterator.hasNext()) {
        iterator.next();
        if (iterator.key() == m_lightTheme || iterator.key() == m_darkTheme) continue;

        QJsonObject theme;
        theme["name"] = iterator.key();
        auto baseThemeName = iterator.value()->value(basedOnThemeField);
        theme[basedOnThemeField] = baseThemeName;
        auto baseTheme = m_themes.value(baseThemeName);
        QMapIterator<QString, QString> fieldsIterator(*iterator.value());
        while (fieldsIterator.hasNext()) {
            fieldsIterator.next();
            auto key = fieldsIterator.key();
            //if base theme already contains value don't need save field
            if (baseTheme->value(key) == fieldsIterator.value()) continue;

            theme[key] = fieldsIterator.value();
        }
        themes.append(theme);
    }

    QJsonObject root;
    root["selectedTheme"] = m_selectedTheme;
    root["themes"] = themes;

    QFile cacheFile(m_cachePathName);
    if (!cacheFile.open(QFile::WriteOnly | QFile::Text)) return;

    auto document = QJsonDocument(root);
    cacheFile.write(document.toJson());
    cacheFile.close();
}

void ApplicationThemeViewModel::reloadThemes()
{
    auto oldSelectedTheme = m_selectedTheme;
    readCacheFile();
    if (m_selectedTheme != oldSelectedTheme) {
        if (m_themes.contains(oldSelectedTheme)) setSelectedTheme(oldSelectedTheme);
    }
}

void ApplicationThemeViewModel::importTheme(const QString &content)
{
    auto jsonDocument = QJsonDocument::fromJson(content.toUtf8());
    auto rootObject = jsonDocument.object();

    if (!rootObject.contains("name")) {
        emit errorImportTheme("Тема не импортирована, в ней отсутствует поле name");
        return;
    }
    if (!rootObject.contains(basedOnThemeField)) {
        emit errorImportTheme("Тема не импортирована, в ней отсутствует поле base");
        return;
    }

    auto themeName = rootObject.value("name").toString();
    auto baseName = rootObject.value(basedOnThemeField).toString();

    if (baseName != m_lightTheme && baseName != m_darkTheme) {
        emit errorImportTheme("Тема не импортирована, поле base содержит не корректное значение");
        return;
    }

    auto importedTheme = new QMap<QString, QString>();
    auto baseTheme = m_themes.value(baseName);

    importedTheme->insert(basedOnThemeField, baseName);

    foreach (auto field, m_fields) {
        setThemeValue(importedTheme, rootObject, baseTheme, field);
    }

    if (m_themes.contains(themeName)) {
        m_themes[themeName] = importedTheme;
    } else {
        m_themes.insert(themeName, importedTheme);
        m_localIds->append(themeName);
    }
}

void ApplicationThemeViewModel::importThemeFromFile(const QString &content)
{
    QString path = content;
    auto clearedPath = removeFileProtocol(path);
    QFile file(clearedPath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) return;
    auto fileContent = file.readAll();
    file.close();

    importTheme(fileContent);
    emit themesChanged();
    m_localThemes->refresh();
    emit copyMenuItemsChanged();
}

void ApplicationThemeViewModel::importThemeFromExternal(int themeIndex)
{
    auto theme =  m_externalThemes->getThemeByIndex(themeIndex);
    m_service->downloadTheme(theme);
}

void ApplicationThemeViewModel::saveThemeAndApply() noexcept
{
    auto values = m_fieldList->getValues();
    auto basedOnTheme = m_fieldList->basedOnTheme();
    auto name =  m_fieldList->themeName();

    auto newTheme = new QMap<QString, QString>();
    newTheme->insert(basedOnThemeField, basedOnTheme);
    newTheme->insert(externalIdField, "");

    auto baseTheme = m_themes.value(basedOnTheme);

    foreach (auto field, m_fields) {
        if (values.contains(field) && !values.value(field).isEmpty()) {
            newTheme->insert(field, values.value(field));
        } else {
            newTheme->insert(field, baseTheme->value(field));
        }
    }

    if (m_themes.contains(name)) {
        m_themes[name] = newTheme;
    } else {
        m_themes.insert(name, newTheme);
    }

    emit themesChanged();
    if (!m_localIds->contains(name)) m_localIds->append(name);
    m_localThemes->refresh();
    emit copyMenuItemsChanged();

    if (m_selectedTheme == name) {
        emitAllFields();
        setCurrentItems();
    }
}

void ApplicationThemeViewModel::preparePreviewItems() noexcept
{
    emit previewItemsChanged();
}

void ApplicationThemeViewModel::deleteThemeByExternalId(const QString &externalId) noexcept
{
    QString themeName;
    QMapIterator<QString, QMap<QString, QString>*> iterator(m_themes);
    while (iterator.hasNext()) {
        iterator.next();

        auto value = iterator.value();
        if (!value->contains(externalIdField)) continue;

        if (value->value(externalIdField) == externalId) {
            themeName = iterator.key();
        }
    }
    if (!themeName.isEmpty()) m_themes.remove(themeName);
    if (m_selectedTheme == themeName) setSelectedTheme(m_lightTheme);

    if (m_externalIds->contains(externalId)) m_externalIds->removeOne(externalId);
    m_externalThemes->refresh();
    emit themesChanged();
    emit copyMenuItemsChanged();
}

void ApplicationThemeViewModel::deleteThemeFromLocal(const QString &name) noexcept
{
    if (name == m_lightTheme || name == m_darkTheme) return;
    if (!m_themes.contains(name)) return;
    auto theme = m_themes.value(name);
    if (theme->contains(externalIdField) && !theme->value(externalIdField).isEmpty()) return;

    m_themes.remove(name);
    if (m_selectedTheme == name) setSelectedTheme(m_lightTheme);
    emit themesChanged();
    m_localIds->removeOne(name);
    m_localThemes->refresh();
    emit copyMenuItemsChanged();
}

void ApplicationThemeViewModel::copyThemeFromInstalled(const QString &name) noexcept
{
    if (name == m_lightTheme || name == m_darkTheme) return;
    if (!m_themes.contains(name)) return;

    auto theme = m_themes.value(name);
    auto baseThemeName = theme->value(basedOnThemeField);
    auto baseTheme = m_themes[baseThemeName];

    QMap<QString, QString> m_copiedTheme;

    foreach (auto field, m_fields) {
        if (field == externalIdField) continue;
        auto fieldValue = theme->value(field);
        if (fieldValue == baseTheme->value(field)) continue;

        m_copiedTheme.insert(field, fieldValue);
    }

    m_fieldList->setValues(std::move(m_copiedTheme), name + (m_notAddCopyToName ? "" : " Копия"), baseThemeName);
}

void ApplicationThemeViewModel::readCacheFile()
{
    QFile cacheFile(m_cachePathName);
    if (!cacheFile.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }
    auto data = cacheFile.readAll();
    cacheFile.close();

    auto jsonDocument = QJsonDocument::fromJson(data);
    auto rootObject = jsonDocument.object();

    QString selectedTheme = m_lightTheme;
    if (rootObject.contains("selectedTheme")) selectedTheme = rootObject.value("selectedTheme").toString();

    if (!rootObject.contains("themes")) return;

    auto themes = rootObject.value("themes").toArray();
    foreach (auto theme, themes) {
        auto themeItem = theme.toObject();
        if (!themeItem.contains("name")) continue;
        if (!themeItem.contains(basedOnThemeField)) continue;

        auto themeName = themeItem.value("name").toString();
        auto baseName = themeItem.value(basedOnThemeField).toString();

        if (baseName != m_lightTheme && baseName != m_darkTheme) continue;

        auto baseTheme = m_themes.value(baseName);

        auto savedTheme = new QMap<QString, QString>();

        savedTheme->insert(basedOnThemeField, baseName);

        foreach (auto field, m_fields) {
            setThemeValue(savedTheme, themeItem, baseTheme, field);
        }

        m_themes.insert(themeName, savedTheme);
    }

    setSelectedTheme(selectedTheme);
    if (m_currentItems.isEmpty()) setCurrentItems();

    m_externalIds->clear();
    QMapIterator<QString, QMap<QString, QString>*> iterator(m_themes);
    while (iterator.hasNext()) {
        iterator.next();

        auto value = iterator.value();
        auto isExternal = value->contains(externalIdField) && !value->value(externalIdField).isEmpty();
        if (isExternal) {
            m_externalIds->append(value->value(externalIdField));
        } else {
            auto themeName = iterator.key();
            if (themeName != m_lightTheme && themeName != m_darkTheme) m_localIds->append(themeName);
        }
    }

    emit copyMenuItemsChanged();
}

void ApplicationThemeViewModel::setThemeValue(QMap<QString, QString>* theme,const QJsonObject &themeItem, const QMap<QString, QString> *baseTheme, const QString& name)
{
    theme->insert(name, themeItem.contains(name) ? themeItem.value(name).toString() : baseTheme->value(name));
}

void ApplicationThemeViewModel::emitAllFields()
{
    emit basedOnDarkChanged();
    emit selectedThemeChanged();
    emit plainTextColorChanged();
    emit headerTextColorChanged();
    emit linkTextColorChanged();
    emit pageVerticalPanelChanged();
    emit pageBackgroundChanged();
    emit pageUpperPanelChanged();
    emit panelBackgroundChanged();
    emit panelBackgroundShadowChanged();
    emit roundedButtonBackgroundChanged();
    emit roundedButtonBackgroundDisabledChanged();
    emit roundedButtonForegroundChanged();
    emit roundedButtonHoveredChanged();
    emit drawerGradiendStep0Changed();
    emit drawerGradiendStep1Changed();
    emit drawerGradiendStep2Changed();
    emit drawerGradiendStep3Changed();
    emit drawerGradiendStep4Changed();
    emit filterIconButtonColorChanged();
    emit filterIconButtonGreenColorChanged();
    emit filterIconButtonHoverColorChanged();
    emit selectedItemChanged();
    emit selectedFavoriteChanged();
    emit playerControlBackgroundChanged();
    emit notificationCenterBackgroundChanged();
    emit notificationCenterPanelBackgroundChanged();
    emit notificationCenterItemBackgroundChanged();
    emit playlistSelectedBackgroundChanged();
    emit playlistBackgroundChanged();
    emit playlistSelectedTextChanged();
    emit playlistTextChanged();
    emit toggleButtonHoveredChanged();
    emit toggleButtonCheckedChanged();
    emit toggleButtonNotCheckedChanged();
    emit materialAccentChanged();
    emit roundedButtonDisableForegroundChanged();
    emit iconMainMenuChanged();
    emit iconMainMenuReleasesChanged();
    emit iconMainMenuReleasesSeriesChanged();
    emit iconMainMenuCinemahallChanged();
    emit iconMainMenuMyAnilibriaChanged();
    emit iconMainMenuMaintenanceChanged();
    emit iconMainMenuYoutubeChanged();
    emit iconMainMenuDonateChanged();
    emit iconMainMenuThemeManagerChanged();
    emit iconMainMenuVideoplayerChanged();
    emit iconLeftHalfScreenChanged();
    emit iconRightHalfScreenChanged();
    emit iconMinimizeWindowChanged();
    emit iconMaximizeWindowChanged();
    emit iconCloseWindowChanged();
    emit iconNormalWindowChanged();
    emit iconMainMenuAuthorizationChanged();
    emit colorMaterialTextChanged();
    emit colorBackgroundNavigationButtonChanged();
    emit colorPageIndexTextChanged();
}

void ApplicationThemeViewModel::themesLoaded()
{
    m_externalThemes->setItems(m_service->getItems(), m_externalIds);
}

void ApplicationThemeViewModel::themeLoaded(const QString &theme, bool isDark, const ThemeItemModel* externalTheme)
{
    auto themeName = externalTheme->name();
    auto baseName = isDark ? m_darkTheme : m_lightTheme;

    auto jsonDocument = QJsonDocument::fromJson(theme.toUtf8());
    auto rootObject = jsonDocument.object();

    auto importedTheme = new QMap<QString, QString>();
    auto baseTheme = m_themes.value(baseName);

    importedTheme->insert(basedOnThemeField, baseName);
    importedTheme->insert(externalIdField, externalTheme->source());

    foreach (auto field, m_fields) {
        if (field == externalIdField) continue;

        setThemeValue(importedTheme, rootObject, baseTheme, field);
    }

    m_themes.insert(themeName, importedTheme);

    emit themesChanged();
    m_externalIds->append(externalTheme->source());
    m_externalThemes->refresh();

    emit copyMenuItemsChanged();
}
