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

#ifndef GLOBALCONSTANTS_H
#define GLOBALCONSTANTS_H

#include <QString>

extern QString AnilibriaImagesPath;

extern QString AnilibriaApiPath;

extern bool IsPortable;

const QString ApplicationVersion = "1.2.8";

const int HistoryReleaseCardMode = 0;

const int HistoryWatchReleaseCardMode = 1;

extern QString StatisticsSectionId;
extern QString NewInFavoritesSectionId;
extern QString NewFromStartSectionId;
extern QString LastTwoDaysSectionId;
extern QString AbandonedSeensSectionId;
extern QString RecommendedByGenresSectionId;
extern QString NextInReleaseSeriesSectionId;
extern QString CurrentSeasonSectionId;
extern QString ActualInCurrentSeasonSectionId;
extern QString RecommendedByVoicesSectionId;
extern QString WillWatchSectionId;

extern QString StatisticsSectionComponent;

extern QString SimpleListSectionComponent;

extern int MovingUp;

extern int MovingDown;

extern QString plainTextColorField;

extern QString headerTextColorField;

extern QString linkTextColorField;

extern QString pageVerticalPanelField;

extern QString pageBackgroundField;

extern QString pageUpperPanelField;

extern QString panelBackgroundField;

extern QString panelBackgroundShadowField;

extern QString roundedButtonBackgroundField;

extern QString roundedButtonBackgroundDisabledField;

extern QString roundedButtonForegroundField;

extern QString roundedButtonHoveredField;

extern QString roundedButtonDisableForegroundField;

extern QString drawerGradiendStep0Field;

extern QString drawerGradiendStep1Field;

extern QString drawerGradiendStep2Field;

extern QString drawerGradiendStep3Field;

extern QString drawerGradiendStep4Field;

extern QString filterIconButtonColorField;

extern QString filterIconButtonGreenColorField;

extern QString filterIconButtonHoverColorField;

extern QString selectedItemField;

extern QString selectedFavoriteField;

extern QString playerControlBackgroundField;

extern QString notificationCenterBackgroundField;

extern QString notificationCenterPanelBackgroundField;

extern QString notificationCenterItemBackgroundField;

extern QString playlistSelectedBackgroundField;

extern QString playlistBackgroundField;

extern QString playlistSelectedTextField;

extern QString playlistTextField;

extern QString basedOnThemeField;

extern QString externalIdField;

extern QString toggleButtonHoveredField;

extern QString toggleButtonCheckedField;

extern QString toggleButtonNotCheckedField;

extern QString posterBorderField;

extern QString materialAccentField;

extern QString colorBorderInPanelField;

extern QString colorBackgroundNavigationButtonField;

extern QString colorMaterialTextField;

extern QString colorPageIndexTextField;

extern QString colorTooltipTextField;
extern QString colorSeenReleaseItemTextField;

extern QString iconMainMenuField;
extern QString iconMainMenuReleasesField;
extern QString iconMainMenuCinemahallField;
extern QString iconMainMenuReleasesSeriesField;
extern QString iconMainMenuMaintenanceField;
extern QString iconMainMenuMyAnilibriaField;
extern QString iconMainMenuVideoplayerField;
extern QString iconMainMenuDonateField;
extern QString iconMainMenuThemeManagerField;
extern QString iconMainMenuYoutubeField;
extern QString iconMainMenuTorrentStreamField;
extern QString iconMainMenuAuthorizationField;
extern QString iconLeftHalfScreenField;
extern QString iconRightHalfScreenField;
extern QString iconMinimizeWindowField;
extern QString iconMaximizeWindowField;
extern QString iconNormalWindowField;
extern QString iconCloseWindowField;
extern QString iconFavoritesField;
extern QString iconSeenField;
extern QString iconInfoField;
extern QString iconClearTextControlField;
extern QString iconBackToTopField;
extern QString iconReleaseCatalogSynchronizationField;
extern QString iconReleaseCatalogSearchField;
extern QString iconReleaseCatalogNotificationField;
extern QString iconReleaseCatalogRandomField;
extern QString iconReleaseCatalogSettingsField;
extern QString iconReleaseCatalogHidedButtonsField;
extern QString iconReleaseCatalogOnlineField;
extern QString iconReleaseCatalogTorrentField;
extern QString iconReleaseCatalogScheduleField;
extern QString iconReleaseCatalogHistoryField;
extern QString iconReleaseCatalogAlphabetField;
extern QString iconReleaseCatalogAllReleasesField;
extern QString iconReleaseCatalogCompilationField;
extern QString iconReleaseCatalogSortAscField;
extern QString iconReleaseCatalogSortDescField;
extern QString iconReleaseCardListMenuField;
extern QString iconReleaseCardCloseField;
extern QString iconReleaseCardCopyField;
extern QString iconReleaseCardExternalField;
extern QString iconNavigationButtonLeftField;
extern QString iconNavigationButtonRightField;
extern QString iconMyAnilibriaButtonSortUpField;
extern QString iconMyAnilibriaButtonSortBottomField;
extern QString iconMyAnilibriaButtonPlusField;
extern QString iconMyAnilibriaButtonMinusField;
extern QString iconMyAnilibriaButtonSortSectionsField;
extern QString iconPlayerFullscreenField;
extern QString iconPlayerCropModeField;
extern QString iconPlayerSettingsField;
extern QString iconPlayerRemoteField;
extern QString iconPlayerTopMostField;
extern QString iconPlayerRefreshField;
extern QString iconPlayerInfoField;
extern QString iconPlayerMuteField;
extern QString iconPlayerUnMuteField;
extern QString iconPlayerDrawerField;
extern QString iconPlayerSeenField;
extern QString iconPlayerUnseenField;
extern QString iconPlayerExternalField;
extern QString iconPlayerPlayField;
extern QString iconPlayerPauseField;
extern QString iconPlayerJumpLeftField;
extern QString iconPlayerJumpRightField;
extern QString iconPlayerNextSeriaField;
extern QString iconPlayerPreviousSeriaField;
extern QString iconDeleteItemField;
extern QString iconDownloadThemeField;
extern QString iconEditThemeFieldPlusField;
extern QString iconEmptyField;
extern QString iconNotificationField;
extern QString iconNotificationSelectedField;
extern QString colorTooltipBackgroundField;
extern QString colorDrawerItemTextField;
extern QString colorDrawerItemHoveredField;
extern QString colorPosterFilterField;
extern QString colorPopupBackgroundField;
extern QString colorPopupBackgroundShadowField;
extern QString colorSwitchHighlightedRippleField;
extern QString colorSwitchRippleField;
extern QString colorSwitchCheckedTrackField;
extern QString colorSwitchUncheckedTrackField;
extern QString colorSwitchDisabledTrackField;
extern QString colorSwitchCheckedHandleField;
extern QString colorSwitchUncheckedHandleField;
extern QString colorSwitchDisabledHandleField;

extern QString colorTextFieldAccentField;
extern QString colorTextFieldForegroundField;
extern QString colorTextFieldHintField;
extern QString colorTextFieldHighlightField;
extern QString colorTextFieldTextField;

extern QString colorMenuDialogBackgroundField;
extern QString colorMenuOverlayBackgroundField;
extern QString colorMenuForegroundField;
extern QString colorMenuItemHintField;
extern QString colorMenuRippleField;
extern QString colorMenuHighlightField;

extern QString iconLogoutField;
extern QString colorDrawerDividerColorField;
extern QString colorDrawerDimColorField;
extern QString colorDrawerDialogColorField;

extern QString iconDisabledNotificationsField;
extern QString iconDisabledCinemahallField;

#endif // GLOBALCONSTANTS_H
