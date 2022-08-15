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

const QString ApplicationVersion = "1.1.11";

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

#endif // GLOBALCONSTANTS_H
