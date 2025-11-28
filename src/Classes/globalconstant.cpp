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

#include "globalconstants.h"

QString AnilibriaImagesPath = "https://anilibriaqt.anilib.top";
QString AnilibriaApiPath = "https://anilibriaqt.anilib.top/";
bool IsPortable = false;
QString ApplicationVersion = "2.2.33";

int HistoryReleaseCardMode = 0;
int HistoryWatchReleaseCardMode = 1;

QString StatisticsSectionId = "statistics";
QString NewInFavoritesSectionId = "newinfavorites";
QString NewFromStartSectionId = "newfromstart";
QString LastTwoDaysSectionId = "lasttwodays";
QString AbandonedSeensSectionId = "abondonedseens";
QString RecommendedByGenresSectionId = "recommendbygenres";
QString WillWatchSectionId = "willwatch";
QString NextInReleaseSeriesSectionId = "nextinreleaseseries";
QString CurrentSeasonSectionId = "currentseason";
QString ActualInCurrentSeasonSectionId = "actualincurrentseason";
QString RecommendedByVoicesSectionId = "recommendedbyvoices";

QString StatisticsSectionComponent = "StatisticsSection";
QString SimpleListSectionComponent = "SimpleListSection";

int MovingUp = 1;
int MovingDown = -1;

QString plainTextColorField = "plainTextColor";
QString headerTextColorField = "headerTextColor";
QString linkTextColorField = "linkTextColor";
QString pageVerticalPanelField = "pageVerticalPanel";
QString pageBackgroundField = "pageBackground";
QString pageUpperPanelField = "pageUpperPanel";
QString panelBackgroundField = "panelBackground";
QString panelBackgroundShadowField = "panelBackgroundShadow";
QString roundedButtonBackgroundField = "roundedButtonBackground";
QString roundedButtonBackgroundDisabledField = "roundedButtonBackgroundDisabled";
QString roundedButtonForegroundField = "roundedButtonForeground";
QString roundedButtonHoveredField = "roundedButtonHovered";
QString drawerGradiendStep0Field = "drawerGradiendStep0";
QString drawerGradiendStep1Field = "drawerGradiendStep1";
QString drawerGradiendStep2Field = "drawerGradiendStep2";
QString drawerGradiendStep3Field = "drawerGradiendStep3";
QString drawerGradiendStep4Field = "drawerGradiendStep4";
QString filterIconButtonColorField = "filterIconButtonColor";
QString filterIconButtonGreenColorField = "filterIconButtonGreenColor";
QString filterIconButtonHoverColorField = "filterIconButtonHoverColor";
QString selectedItemField = "selectedItem";
QString selectedFavoriteField = "selectedFavorite";
QString playerControlBackgroundField = "playerControlBackground";
QString notificationCenterBackgroundField = "notificationCenterBackground";
QString notificationCenterPanelBackgroundField = "notificationCenterPanelBackground";
QString notificationCenterItemBackgroundField = "notificationCenterItemBackground";
QString playlistSelectedBackgroundField = "playlistSelectedBackground";
QString playlistBackgroundField = "playlistBackground";
QString playlistSelectedTextField = "playlistSelectedText";
QString playlistTextField = "playlistText";
QString basedOnThemeField = "basedOnTheme";
QString externalIdField = "externalId";
QString toggleButtonHoveredField = "toggleButtonHovered";
QString toggleButtonCheckedField = "toggleButtonChecked";
QString toggleButtonNotCheckedField = "toggleButtonNotChecked";
QString posterBorderField = "posterBorder";
QString roundedButtonDisableForegroundField = "roundedButtonDisableForeground";
QString materialAccentField = "materialAccent";
QString colorBorderInPanelField = "colorBorderInPanel";
QString colorBackgroundNavigationButtonField = "colorBackgroundNavigationButton";
QString colorMaterialTextField = "colorMaterialText";
QString colorPageIndexTextField = "colorPageIndexText";
QString colorTooltipTextField = "colorTooltipText";
QString colorSeenReleaseItemTextField = "colorSeenReleaseItemText";
QString iconMainMenuField = "iconMainMenu";
QString iconMainMenuReleasesField = "iconMainMenuReleases";
QString iconMainMenuCinemahallField = "iconMainMenuCinemahall";
QString iconMainMenuReleasesSeriesField = "iconMainMenuReleasesSeries";
QString iconMainMenuMaintenanceField = "iconMainMenuMaintenance";
QString iconMainMenuMyAnilibriaField = "iconMainMenuMyAnilibria";
QString iconMainMenuVideoplayerField = "iconMainMenuVideoplayer";
QString iconMainMenuDonateField = "iconMainMenuDonate";
QString iconMainMenuThemeManagerField = "iconMainMenuThemeManager";
QString iconMainMenuYoutubeField = "iconMainMenuYoutube";
QString iconMainMenuTorrentStreamField = "iconMainMenuTorrentStream";
QString iconMainMenuApplicationsField = "iconMainMenuApplications";
QString iconLeftHalfScreenField = "iconLeftHalfScreen";
QString iconRightHalfScreenField = "iconRightHalfScreen";
QString iconMinimizeWindowField = "iconMinimizeWindow";
QString iconMaximizeWindowField = "iconMaximizeWindow";
QString iconNormalWindowField = "iconNormalWindow";
QString iconCloseWindowField = "iconCloseWindow";
QString iconMainMenuAuthorizationField = "iconMainMenuAuthorization";
QString iconFavoritesField = "iconFavorites";
QString iconSeenField = "iconSeen";
QString iconInfoField = "iconInfo";
QString iconBackToTopField = "iconBackToTop";
QString iconClearTextControlField = "iconClearTextControl";
QString iconReleaseCatalogSynchronizationField = "iconReleaseCatalogSynchronization";
QString iconReleaseCatalogSearchField = "iconReleaseCatalogSearch";
QString iconReleaseCatalogNotificationField = "iconReleaseCatalogNotification";
QString iconReleaseCatalogRandomField = "iconReleaseCatalogRandom";
QString iconReleaseCatalogSettingsField = "iconReleaseCatalogSettings";
QString iconReleaseCatalogHidedButtonsField = "iconReleaseCatalogHidedButtons";
QString iconReleaseCatalogOnlineField = "iconReleaseCatalogOnline";
QString iconReleaseCatalogTorrentField = "iconReleaseCatalogTorrent";
QString iconReleaseCatalogScheduleField = "iconReleaseCatalogSchedule";
QString iconReleaseCatalogHistoryField = "iconReleaseCatalogHistory";
QString iconReleaseCatalogAlphabetField = "iconReleaseCatalogAlphabet";
QString iconReleaseCatalogAllReleasesField = "iconReleaseCatalogAllReleases";
QString iconReleaseCatalogCompilationField = "iconReleaseCatalogCompilation";
QString iconReleaseCatalogSortAscField = "iconReleaseCatalogSortAsc";
QString iconReleaseCatalogSortDescField = "iconReleaseCatalogSortDesc";
QString iconReleaseCardListMenuField = "iconReleaseCardListMenu";
QString iconReleaseCardCloseField = "iconReleaseCardClose";
QString iconReleaseCardCopyField = "iconReleaseCardCopy";
QString iconReleaseCardExternalField = "iconReleaseCardExternal";
QString iconNavigationButtonLeftField = "iconNavigationButtonLeft";
QString iconNavigationButtonRightField = "iconNavigationButtonRight";
QString iconMyAnilibriaButtonSortUpField = "iconMyAnilibriaButtonSortUp";
QString iconMyAnilibriaButtonSortBottomField = "iconMyAnilibriaButtonSortBottom";
QString iconMyAnilibriaButtonPlusField = "iconMyAnilibriaButtonPlus";
QString iconMyAnilibriaButtonMinusField = "iconMyAnilibriaButtonMinus";
QString iconMyAnilibriaButtonSortSectionsField = "iconMyAnilibriaButtonSortSections";
QString iconPlayerFullscreenField = "iconPlayerFullscreen";
QString iconPlayerCropModeField = "iconPlayerCropMode";
QString iconPlayerSettingsField = "iconPlayerSettings";
QString iconPlayerRemoteField = "iconPlayerRemote";
QString iconPlayerTopMostField = "iconPlayerTopMost";
QString iconPlayerRefreshField = "iconPlayerRefresh";
QString iconPlayerInfoField = "iconPlayerInfo";
QString iconPlayerMuteField = "iconPlayerMute";
QString iconPlayerUnMuteField = "iconPlayerUnMute";
QString iconPlayerDrawerField = "iconPlayerDrawer";
QString iconPlayerSeenField = "iconPlayerSeen";
QString iconPlayerUnseenField = "iconPlayerUnseen";
QString iconPlayerExternalField = "iconPlayerExternal";
QString iconPlayerPlayField = "iconPlayerPlay";
QString iconPlayerPauseField = "iconPlayerPause";
QString iconPlayerJumpLeftField = "iconPlayerJumpLeft";
QString iconPlayerJumpRightField = "iconPlayerJumpRight";
QString iconPlayerNextSeriaField = "iconPlayerNextSeria";
QString iconPlayerPreviousSeriaField = "iconPlayerPreviousSeria";
QString iconDeleteItemField = "iconDeleteItem";
QString iconDownloadThemeField = "iconDownloadTheme";
QString iconEditThemeFieldPlusField = "iconEditThemeFieldPlus";
QString iconEmptyField = "iconEmpty";
QString iconNotificationField = "iconNotification";
QString iconNotificationSelectedField = "iconNotificationSelected";
QString colorTooltipBackgroundField = "colorTooltipBackground";
QString colorDrawerItemTextField = "colorDrawerItemText";
QString colorDrawerItemHoveredField = "colorDrawerItemHovered";
QString colorPosterFilterField = "colorPosterFilter";
QString colorPopupBackgroundField = "colorPopupBackground";
QString colorPopupBackgroundShadowField = "colorPopupBackgroundShadow";
QString colorSwitchHighlightedRippleField = "colorSwitchHighlightedRipple";
QString colorSwitchRippleField = "colorSwitchRipple";
QString colorSwitchCheckedTrackField = "colorSwitchCheckedTrack";
QString colorSwitchUncheckedTrackField = "colorSwitchUncheckedTrack";
QString colorSwitchDisabledTrackField = "colorSwitchDisabledTrack";
QString colorSwitchCheckedHandleField = "colorSwitchCheckedHandle";
QString colorSwitchUncheckedHandleField = "colorSwitchUncheckedHandle";
QString colorSwitchDisabledHandleField = "colorSwitchDisabledHandle";
QString colorTextFieldAccentField = "colorTextFieldAccent";
QString colorTextFieldForegroundField = "colorTextFieldForeground";
QString colorTextFieldHintField = "colorTextFieldHint";
QString colorTextFieldHighlightField = "colorTextFieldHighlight";
QString colorTextFieldTextField = "colorTextFieldText";

QString colorMenuDialogBackgroundField = "colorMenuDialogBackground";
QString colorMenuOverlayBackgroundField = "colorMenuOverlayBackground";

QString colorMenuForegroundField = "colorMenuForeground";
QString colorMenuItemHintField = "colorMenuItemHint";
QString colorMenuRippleField = "colorMenuRipple";
QString colorMenuHighlightField = "colorMenuHighlight";

QString iconLogoutField = "iconLogout";
QString colorDrawerDividerColorField = "colorDrawerDivider";
QString colorDrawerDimColorField = "colorDrawerDim";
QString colorDrawerDialogColorField = "colorDrawerDialog";

QString iconDisabledNotificationsField = "iconDisabledNotifications";
QString iconDisabledCinemahallField = "iconDisabledCinemahall";

QString colorComboBoxAccentField = "colorComboBoxAccent";
QString colorComboBoxHintField = "colorComboBoxHint";
QString colorComboBoxBackgroundField = "colorComboBoxBackground";

QString iconCustomGroupField = "iconCustomGroup";
QString iconEditField = "iconEdit";
QString iconReleaseCatalogSearchFoundedField = "iconReleaseCatalogSearchFounded";
QString iconCustomGroupFoundedField = "iconCustomGroupFounded";
QString iconAuthorizationVkField = "iconAuthorizationVk";
QString iconAuthorizationGoogleField = "iconAuthorizationGoogle";
QString iconAuthorizationPatreonField = "iconAuthorizationPatreon";
QString iconAuthorizationDiscordField = "iconAuthorizationDiscord";
QString iconContextMenuField = "iconContextMenu";
QString iconMainMenuExtensionsField = "iconMainMenuExtensions";
QString iconMagnetField = "iconMagnet";

QString iconDisableGroupingField = "iconDisableGrouping";
QString iconEnableGroupingField = "iconEnableGrouping";

QString iconSubtitlesPopupField = "iconSubtitlesPopup";

QString backgroundsPagesField = "backgroundsPages";
