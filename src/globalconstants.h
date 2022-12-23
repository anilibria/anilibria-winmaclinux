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

const QString ApplicationVersion = "1.2.3";

const int HistoryReleaseCardMode = 0;

const int HistoryWatchReleaseCardMode = 1;

extern QString StatisticsSectionId;

extern QString NewInFavoritesSectionId;

extern QString NewFromStartSectionId;

extern QString LastTwoDaysSectionId;

extern QString AbandonedSeensSectionId;

extern QString RecommendedByGenresSectionId;

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

extern QString iconMainMenuAuthorizationField;

extern QString iconLeftHalfScreenField;

extern QString iconRightHalfScreenField;

extern QString iconMinimizeWindowField;

extern QString iconMaximizeWindowField;

extern QString iconNormalWindowField;

extern QString iconCloseWindowField;


#endif // GLOBALCONSTANTS_H
