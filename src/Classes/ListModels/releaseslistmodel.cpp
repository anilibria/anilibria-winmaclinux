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

#include <QFile>
#include "releaseslistmodel.h"

const int FavoriteSection = 1;
const int NewReleasesSection = 2;
const int NewOnlineSeriesSection = 3;
const int NewTorrentsSection = 4;
const int ScheduleSection = 5;
const int NewTorrentSeriesSection = 6;
const int HistorySection = 7;
const int WatchHistorySection = 8;
const int SeenHistorySection = 9;
const int SeeningHistorySection = 10;
const int NotSeeningHistorySection = 11;
const int HiddenReleasesSection = 12;
const int OnlyMoviesSection = 14;
const int MostPopularSection = 15;
const int WithinSeriesSection = 16;
const int MostPopular2021Section = 17;
const int FinishedSeenSection = 18;
const int NotFinishedSeenSection = 19;
const int MostPopular2022Section = 20;
const int AddedToCinemahall = 21;
const int CurrentSeasonSection = 22;
const int NotCurrentSeasonSection = 23;
const int CustomScriptSection = 24;
const int PlannedCollectionSection = 25;
const int WatchCollectionSection = 26;
const int WatchedCollectionSection = 27;
const int PostponedSection = 28;
const int AbandonedSection = 29;

const int winter = 0;
const int autumn = 1;
const int spring = 2;
const int summer = 3;

ReleasesListModel::ReleasesListModel(QObject *parent) : QAbstractListModel(parent)
{
}

void ReleasesListModel::setup(QSharedPointer<QList<FullReleaseModel *>> releases, QMap<int, int> *schedules, QList<int> *userFavorites, QList<int> *hidedReleases, QHash<QString, std::tuple<bool, int>>* seenMarks, QSharedPointer<QHash<int, HistoryModel *>> historyItems, QSharedPointer<ChangesModel> changes, QSharedPointer<CinemahallListModel> cinemahall, ReleaseCustomGroupsViewModel* customGroups, QMap<QString, ReleaseOnlineVideoModel*>* videosMap, QMap<int, QString>* collections)
{
    m_releases = releases;
    m_scheduleReleases = schedules;
    m_userFavorites = userFavorites;
    m_hiddenReleases = hidedReleases;
    m_seenMarkModels = seenMarks;
    m_historyModels = historyItems;
    m_changesModel = changes;
    m_cinemahall = cinemahall;
    m_customGroups = customGroups;
    m_videosMap = videosMap;
    m_collections = collections;
}

void ReleasesListModel::setupLinkedSeries(ReleaseLinkedSeries *releaseLinkedSeries) noexcept
{
    m_releaseLinkedSeries = releaseLinkedSeries;
}

int ReleasesListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_filteredReleases.size();
}

QVariant ReleasesListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto release = m_filteredReleases.at(index.row());
    if (release == nullptr) return QVariant();

    switch (role) {
        case ReleaseIdRole: {
            return QVariant(release->id());
        }
        case TitleRole: {
            return QVariant(release->title());
        }
        case StatusRole: {
            return QVariant(release->status());
        }
        case SeasonRole: {
            return QVariant(release->season());
        }
        case YearRole: {
            return QVariant(release->year());
        }
        case TypeRole: {
            return QVariant(release->type() + " " + release->series());
        }
        case GenresRole: {
            return QVariant(release->genres());
        }
        case CountVideosRole: {
            return QVariant(release->countOnlineVideos());
        }
        case CountSeensMarksRole: {
            return QVariant(getReleaseSeenMarkCount(release->id()));
        }
        case DescriptionRole: {
            return QVariant(release->description());
        }
        case PosterRole: {
            return QVariant(release->poster());
        }
        case CountTorrentRole: {
            return QVariant(release->countTorrents());
        }
        case InFavoritesRole: {
            return QVariant(m_userFavorites->contains(release->id()));
        }
        case VoicesRole: {
            return QVariant(release->limitVoicers());
        }
        case RatingRole: {
            return QVariant(release->rating());
        }
        case SelectedRole:{
            return QVariant(m_selectedReleases->contains(release->id()));
        }
        case InScheduleRole: {
            return QVariant(m_scheduleReleases->contains(release->id()));
        }
        case ScheduledDayRole: {
            return m_scheduleReleases->contains(release->id()) ? QVariant(getScheduleDay(m_scheduleReleases->value(release->id()))) : QVariant("");
        }
        case StartInGroupRole: {
            return QVariant(m_startInGroups.contains(release->id()));
        }
        case GroupRole: {
            auto groupValue = release->groupValue();
            if (groupValue.contains(".")) {
                auto index = groupValue.indexOf(".") + 1;
                return QVariant(groupValue.mid(index));
            } else {
                return QVariant(release->groupValue());
            }
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ReleasesListModel::roleNames() const
{
    return {
        {
            ReleaseIdRole,
            "id"
        },
        {
            TitleRole,
            "title"
        },
        {
            StatusRole,
            "status"
        },
        {
            SeasonRole,
            "season"
        },
        {
            YearRole,
            "year"
        },
        {
            TypeRole,
            "type"
        },
        {
            GenresRole,
            "genres"
        },
        {
            VoicesRole,
            "voices"
        },
        {
            CountVideosRole,
            "countVideos"
        },
        {
            CountSeensMarksRole,
            "countSeensSeries"
        },
        {
            DescriptionRole,
            "description"
        },
        {
            PosterRole,
            "poster"
        },
        {
            CountTorrentRole,
            "countTorrents"
        },
        {
            RatingRole,
            "rating"
        },
        {
            InFavoritesRole,
            "inFavorites"
        },
        {
            SelectedRole,
            "inSelected"
        },
        {
            InScheduleRole,
            "inSchedule"
        },
        {
            ScheduledDayRole,
            "scheduledDay"
        },
        {
            StartInGroupRole,
            "startInGroup"
        },
        {
            GroupRole,
            "groupValue"
        }
    };
}

void ReleasesListModel::setTitleFilter(const QString &titleFilter) noexcept
{
    if (m_titleFilter == titleFilter) return;

    m_titleFilter = titleFilter;
    emit titleFilterChanged();
}

void ReleasesListModel::setDescriptionFilter(const QString &descriptionFilter) noexcept
{
    if (m_descriptionFilter == descriptionFilter) return;

    m_descriptionFilter = descriptionFilter;
    emit descriptionFilterChanged();
    emit hasFiltersChanged();
}

void ReleasesListModel::setTypeFilter(const QString &typeFilter) noexcept
{
    if (m_typeFilter == typeFilter) return;

    m_typeFilter = typeFilter;
    emit typeFilterChanged();
    emit hasFiltersChanged();
}

void ReleasesListModel::setYearsFilter(const QString &yearsFilter) noexcept
{
    if (m_yearsFilter == yearsFilter) return;

    m_yearsFilter = yearsFilter;
    emit yearsFilterChanged();
    emit hasFiltersChanged();
}

void ReleasesListModel::setStatusesFilter(const QString &statusesFilter) noexcept
{
    if (m_statusesFilter == statusesFilter) return;

    m_statusesFilter = statusesFilter;
    emit statusesFilterChanged();
    emit hasFiltersChanged();
}

void ReleasesListModel::setSeasonesFilter(const QString &seasonesFilter) noexcept
{
    if (m_seasonesFilter == seasonesFilter) return;

    m_seasonesFilter = seasonesFilter;
    emit seasonesFilterChanged();
    emit hasFiltersChanged();
}

void ReleasesListModel::setGenresFilter(const QString &genresFilter) noexcept
{
    if (m_genresFilter == genresFilter) return;

    m_genresFilter = genresFilter;
    emit genresFilterChanged();
    emit hasFiltersChanged();
}

void ReleasesListModel::setGenresFilterOr(bool genresFilterOr) noexcept
{
    if (m_genresFilterOr == genresFilterOr) return;

    m_genresFilterOr = genresFilterOr;
    emit genresFilterOrChanged();
}

void ReleasesListModel::setVoicesFilter(const QString &voicesFilter) noexcept
{
    if (m_voicesFilter == voicesFilter) return;

    m_voicesFilter = voicesFilter;
    emit voicesFilterChanged();
    emit hasFiltersChanged();
}

void ReleasesListModel::setVoicesFilterOr(bool voicesFilterOr) noexcept
{
    if (m_voicesFilterOr == voicesFilterOr) return;

    m_voicesFilterOr = voicesFilterOr;
    emit voicesFilterOrChanged();
}

void ReleasesListModel::setAlphabetsFilter(QString alphabetsFilter) noexcept
{
    if (m_alphabetsFilter == alphabetsFilter) return;

    m_alphabetsFilter = alphabetsFilter;
    emit alphabetsFilterChanged();
}

void ReleasesListModel::setFavoriteMarkFilter(int favoriteMarkFilter) noexcept
{
    if (m_favoriteMarkFilter == favoriteMarkFilter) return;

    m_favoriteMarkFilter = favoriteMarkFilter;
    emit favoriteMarkFilterChanged();
    emit hasFiltersChanged();
}

void ReleasesListModel::setSeenMarkFilter(int seenMarkFilter) noexcept
{
    if (m_seenMarkFilter == seenMarkFilter) return;

    m_seenMarkFilter = seenMarkFilter;
    emit seenMarkFilterChanged();
    emit hasFiltersChanged();
}

void ReleasesListModel::setSection(int section) noexcept
{
    if (m_section == section) return;

    m_section = section;
    emit sectionChanged();
}

void ReleasesListModel::setSortingField(int sortingField) noexcept
{
    if (m_sortingField == sortingField) return;

    m_sortingField = sortingField;
    emit sortingFieldChanged();
}

void ReleasesListModel::setSortingDescending(bool sortingDescending) noexcept
{
    if (m_sortingDescending == sortingDescending) return;

    m_sortingDescending = sortingDescending;
    emit sortingDescendingChanged();
}

void ReleasesListModel::setIsHasReleases(const bool& isHasReleases) noexcept
{
    if (m_isHasReleases == isHasReleases) return;

    m_isHasReleases = isHasReleases;
    emit isHasReleasesChanged();
}

void ReleasesListModel::setScriptFilePath(const QString &scriptFilePath) noexcept
{
    if (m_scriptFilePath == scriptFilePath) return;

    m_scriptFilePath = scriptFilePath;
    emit scriptFilePathChanged();
}

void ReleasesListModel::refreshItem(int id)
{
    refreshFilteredReleaseById(id);
}

int ReleasesListModel::getReleaseSeenMarkCount(int releaseId) const noexcept
{
    auto result = 0;
    auto keys = m_seenMarkModels->keys();
    foreach (auto key, keys) {
        auto item = m_seenMarkModels->value(key);
        bool mark = std::get<0>(item);
        if (!mark) continue;

        if (!m_videosMap->contains(key)) continue;
        auto video = m_videosMap->value(key);
        if (video->releaseId() == releaseId) {
            result += 1;
        }
    }

    return result;
}

QMap<int,int> ReleasesListModel::getReleasesSeenMarkCount(QList<int> releaseIds) const noexcept
{
    QMap<int,int> countSeens;
    foreach (auto releaseId, releaseIds) {
        countSeens.insert(releaseId, 0);
    }

    auto keys = m_seenMarkModels->keys();
    foreach (auto key, keys) {
        auto item = m_seenMarkModels->value(key);
        bool mark = std::get<0>(item);
        if (!mark) continue;

        if (!m_videosMap->contains(key)) continue;
        auto video = m_videosMap->value(key);
        auto releaseId = video->releaseId();
        if (releaseIds.contains(releaseId)) {
            countSeens[releaseId] += 1;
        }
    }

    return countSeens;
}

void ReleasesListModel::setHasReleaseSeriesFilter(bool hasReleaseSeriesFilter) noexcept
{
    if (m_hasReleaseSeriesFilter == hasReleaseSeriesFilter) return;

    m_hasReleaseSeriesFilter = hasReleaseSeriesFilter;
    emit hasReleaseSeriesFilterChanged();
    emit hasFiltersChanged();
}

void ReleasesListModel::setScheduleDayFilter(const QString &scheduleDayFilter) noexcept
{
    if (m_scheduleDayFilter == scheduleDayFilter) return;

    m_scheduleDayFilter = scheduleDayFilter;
    emit scheduleDayFilterChanged();
    emit hasFiltersChanged();
}

QString ReleasesListModel::getScheduleDay(int dayNumber) const noexcept
{
    switch (dayNumber){
        case 1: return QString("понедельник");
        case 2: return QString("вторник");
        case 3: return QString("среда");
        case 4: return QString("четверг");
        case 5: return QString("пятница");
        case 6: return QString("суббота");
        case 7: return QString("воскресенье");
        default: return "";
    }
}

int ReleasesListModel::getScheduleDayNumber(const QString &day) const noexcept
{
    auto loweredDay = day.toLower();
    if (QString("понедельник").contains(loweredDay)) return 1;
    if (QString("вторник").contains(loweredDay)) return 2;
    if (QString("среда").contains(loweredDay)) return 3;
    if (QString("четверг").contains(loweredDay)) return 4;
    if (QString("пятница").contains(loweredDay)) return 5;
    if (QString("суббота").contains(loweredDay)) return 6;
    if (QString("воскресенье").contains(loweredDay)) return 7;

    return -1;
}

void ReleasesListModel::setFilterByFavorites(bool filterByFavorites) noexcept
{
    if (m_filterByFavorites == filterByFavorites) return;

    m_filterByFavorites = filterByFavorites;
    emit filterByFavoritesChanged();
}

bool ReleasesListModel::hasFilters() const noexcept
{
    return !m_descriptionFilter.isEmpty() || !m_genresFilter.isEmpty() ||
        !m_typeFilter.isEmpty() || !m_voicesFilter.isEmpty() ||
        !m_yearsFilter.isEmpty() || !m_seasonesFilter.isEmpty() ||
        !m_statusesFilter.isEmpty() || !m_scheduleDayFilter.isEmpty() ||
        m_hasReleaseSeriesFilter || m_favoriteMarkFilter > 0 || m_seenMarkFilter > 0;
}

void ReleasesListModel::setGrouping(bool grouping) noexcept
{
    if (m_grouping == grouping) return;

    m_grouping = grouping;
    emit groupingChanged();
}

int ReleasesListModel::getReleaseIdByIndex(int index) noexcept
{
    if (index >= m_filteredReleases.count()) return -1;

    auto release = m_filteredReleases.at(index);
    return release->id();
}

QSharedPointer<QSet<int>> ReleasesListModel::getSelectedReleases()
{
    return m_selectedReleases;
}

QString ReleasesListModel::getCurrentSeason()
{
    auto currentYear = QString::number(QDate::currentDate().year());
    QList<int> seasonsCounters;
    for (int i = 0; i < 4; i++) seasonsCounters.append(0);

    foreach (auto release, *m_releases) {
        if (!m_scheduleReleases->contains(release->id())) continue;
        if (release->status().toLower() != "в работе") continue;
        if (release->year() != currentYear) continue;

        auto season = release->season().toLower();
        if (season == winterValue) seasonsCounters[winter]++;
        if (season == summerValue) seasonsCounters[summer]++;
        if (season == autumnValue) seasonsCounters[autumn]++;
        if (season == springValue) seasonsCounters[spring]++;
    }

    auto maxIndex = -1;
    auto maxIndexValue = 0;
    for (int i = 0; i < 4; i++) {
        auto value = seasonsCounters.value(i);
        if (value > maxIndexValue) {
            maxIndexValue = value;
            maxIndex = i;
        }
    }

    if (maxIndex == winter) return winterValue;
    if (maxIndex == autumn) return autumnValue;
    if (maxIndex == spring) return springValue;
    if (maxIndex == summer) return summerValue;

    return "";
}

QList<QList<int> > ReleasesListModel::getFullLinkedReleases()
{
    if (m_releaseLinkedSeries == nullptr) return QList<QList<int>>();

    return m_releaseLinkedSeries->getFullLinkedReleases();
}

void ReleasesListModel::refresh()
{
    beginResetModel();

    m_filteredReleases.clear();
    m_startInGroups.clear();

    auto currentYear = QString::number(QDate::currentDate().year());
    auto currentSeason = getCurrentSeason();

    QSharedPointer<QList<int>> linkedReleases = nullptr;
    if (m_releaseLinkedSeries != nullptr) linkedReleases = m_releaseLinkedSeries->getAllLinkedReleases();

    QHash<int, int> hash1;
    auto seenMarks = getAllSeenMarkCount(std::move(hash1));
    QStringList yearsFilters;
    if (!m_yearsFilter.isEmpty()) {
        yearsFilters = m_yearsFilter.split(",");
        removeTrimsInStringCollection(yearsFilters);
    }

    QStringList statusesFilter;
    if (!m_statusesFilter.isEmpty()) {
        statusesFilter = m_statusesFilter.split(",");
        removeTrimsInStringCollection(statusesFilter);
    }

    QStringList seasonesFilter;
    if (!m_seasonesFilter.isEmpty()) {
        seasonesFilter = m_seasonesFilter.split(",");
        removeTrimsInStringCollection(seasonesFilter);
    }

    QStringList genresFilter;
    if (!m_genresFilter.isEmpty()) {
        genresFilter = m_genresFilter.split(",");
        removeTrimsInStringCollection(genresFilter);
    }

    QStringList voicesFilter;
    if (!m_voicesFilter.isEmpty()) {
        voicesFilter = m_voicesFilter.split(",");
        removeTrimsInStringCollection(voicesFilter);
    }
    QString filterScript = "";
    if (!m_scriptFilePath.isEmpty() && m_section == CustomScriptSection) {
        QFile file(m_scriptFilePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            filterScript = file.readAll();
            file.close();
        }        
    }

    bool isShowedScriptError = false;

    foreach (auto release, *m_releases) {
        auto collectionValue = m_collections->contains(release->id()) ? m_collections->value(release->id()) : "";

        if (m_hiddenReleases->contains(release->id()) && m_section != HiddenReleasesSection) continue;
        if (m_grouping) {
            auto group = getGroupByRelease(release, seenMarks);
            if (group.isEmpty()) continue;

            release->setGroupValue(group);
        } else {
            release->setGroupValue("");
        }
        if (!m_titleFilter.isEmpty()) {
            auto filteredTitle = m_titleFilter.toLower().replace("ё", "е").trimmed();
            auto inTitle = release->title().toLower().replace("ё", "е").contains(filteredTitle);
            auto inOriginalTitle = release->originalName().toLower().replace("ё", "е").contains(filteredTitle);
            if (!(inTitle || inOriginalTitle)) continue;
        }
        if (!m_descriptionFilter.isEmpty() && !release->description().toLower().contains(m_descriptionFilter.toLower())) continue;
        if (!m_typeFilter.isEmpty() && !release->type().toLower().contains(m_typeFilter.toLower())) continue;

        //years
        if (!yearsFilters.isEmpty()) {
            int year = release->year().toInt();
            QStringList singleYear;
            singleYear.append(QString::number(year));

            if (!checkOrCondition(yearsFilters, singleYear)) continue;
        }

        //statuses
        if (!statusesFilter.isEmpty()) {
            QStringList singleStatus;
            singleStatus.append(release->status());

            if (!checkOrCondition(statusesFilter, singleStatus)) continue;
        }

        //seasons
        if (!seasonesFilter.isEmpty()) {
            auto season = release->season();
            QStringList singleSeason;
            singleSeason.append(season);

            if (!checkOrCondition(seasonesFilter, singleSeason)) continue;
        }

        //genres
        if (!genresFilter.isEmpty()) {
            QStringList releaseGenresList = release->genres().split(",");
            if (m_genresFilterOr) {
                if (!checkAllCondition(genresFilter, releaseGenresList)) continue;
            } else {
                if (!checkOrCondition(genresFilter, releaseGenresList)) continue;
            }
        }

        //voices
        if (!voicesFilter.isEmpty()) {
            QStringList releaseVoicesList = release->voicers().split(",");
            if (m_voicesFilterOr) {
                if (!checkAllCondition(voicesFilter, releaseVoicesList)) continue;
            } else {
                if (!checkOrCondition(voicesFilter, releaseVoicesList)) continue;
            }
        }

        //alphabets
        if (!m_alphabetsFilter.isEmpty()) {
            bool startWithAlphabetsCharacters = false;
            foreach (auto alphabetCharacter, m_alphabetsFilter) {
                if (release->title().startsWith(alphabetCharacter)) {
                    startWithAlphabetsCharacters = true;
                }
            }
            if (!startWithAlphabetsCharacters) continue;
        }

        //scheduleDays
        if (!m_scheduleDayFilter.isEmpty()) {
            auto days = m_scheduleDayFilter.split(",");
            auto scheduleDays = QSet<int>();
            foreach (auto day, days) {
                scheduleDays.insert(getScheduleDayNumber(day.trimmed()));
            }
            auto scheduleDay = m_scheduleReleases->value(release->id());
            if (!scheduleDays.contains(scheduleDay)) continue;
        }

        // part of releases series
        if (m_hasReleaseSeriesFilter && linkedReleases != nullptr && !(linkedReleases->contains(release->id()))) continue;

        //favorite mark
        auto inFavorites = m_userFavorites->contains(release->id());

        if (m_favoriteMarkFilter == 1 && !inFavorites) continue;
        if (m_favoriteMarkFilter == 2 && inFavorites) continue;

        //seen mark
        auto countVideos = seenMarks.contains(release->id()) ? seenMarks.value(release->id()) : 0;
        int seenState = countVideos == release->countOnlineVideos() ? 0 : (countVideos > 0 ? 1 : 2);
        if (m_seenMarkFilter == 1 && !(seenState == 0)) continue;
        if (m_seenMarkFilter == 2 && !(seenState == 1)) continue;
        if (m_seenMarkFilter == 3 && !(seenState == 2)) continue;

        //favorites section

        if (m_section == FavoriteSection && !inFavorites) continue;

        if (m_section == ScheduleSection && !m_scheduleReleases->contains(release->id())) continue;

        if (m_section == NewReleasesSection && !m_changesModel->newReleases()->contains(release->id())) continue;

        if (m_section == NewOnlineSeriesSection && !m_changesModel->newOnlineSeries()->contains(release->id())) continue;

        if (m_section == NewTorrentsSection && !m_changesModel->newTorrents()->contains(release->id())) continue;

        if (m_section == NewTorrentSeriesSection && !m_changesModel->newTorrentSeries()->contains(release->id())) continue;

        if ((m_section == NewOnlineSeriesSection ||
           m_section == NewTorrentsSection ||
           m_section == NewTorrentSeriesSection) && m_filterByFavorites && !inFavorites) continue;

        if (m_section == HistorySection && !(m_historyModels->contains(release->id()) && m_historyModels->value(release->id())->timestamp() > 0)) continue;

        if (m_section == WatchHistorySection && !(m_historyModels->contains(release->id()) && m_historyModels->value(release->id())->watchTimestamp() > 0)) continue;

        auto countReleaseSeenVideos = seenMarks.contains(release->id()) ? seenMarks.value(release->id()) : 0;
        auto isAllSeens = countReleaseSeenVideos == release->countOnlineVideos() && release->countOnlineVideos() > 0;
        if (m_section == SeenHistorySection && !isAllSeens) continue;

        if (m_section == SeeningHistorySection && !(countReleaseSeenVideos > 0 && !isAllSeens)) continue;

        if (m_section == NotSeeningHistorySection && !(countReleaseSeenVideos == 0)) continue;

        if (m_section == OnlyMoviesSection && !(release->title().toLower().contains("фильм") || release->type().toLower().contains("фильм"))) continue;

        if (m_section == MostPopularSection && !(release->rating() > 1000)) continue;

        if (m_section == WithinSeriesSection && linkedReleases != nullptr && !(linkedReleases->contains(release->id()))) continue;

        if (m_section == MostPopular2021Section && !(release->status().toLower() == releaseIsFinished)) continue;

        if (m_section == MostPopular2022Section && !(release->year() == "2022" && release->rating() > 0)) continue;

        if (m_section == AddedToCinemahall && !(m_cinemahall->isReleaseInCinemahall(release->id()))) continue;

        if (m_section == HiddenReleasesSection && !m_hiddenReleases->contains(release->id())) continue;

        if (m_section == FinishedSeenSection && !(isAllSeens && release->status().toLower() == releaseIsFinished)) continue;

        if (m_section == NotFinishedSeenSection && !(isAllSeens && release->status().toLower() != releaseIsFinished)) continue;

        if (m_section == CurrentSeasonSection &&
            !(release->year() == currentYear && release->status().toLower() == "в работе" && release->season() == currentSeason)) continue;

        if (m_section == NotCurrentSeasonSection &&
            !((release->year() != currentYear || (release->year() == currentYear && release->season() != currentSeason)) && release->status().toLower() == "в работе")) continue;

        if (m_section == PlannedCollectionSection && collectionValue != "PLANNED") continue;
        if (m_section == WatchCollectionSection && collectionValue != "WATCHING") continue;
        if (m_section == WatchedCollectionSection && collectionValue != "WATCHED") continue;
        if (m_section == PostponedSection && collectionValue != "POSTPONED") continue;
        if (m_section == AbandonedSection && collectionValue != "ABANDONED") continue;

        if (m_section == CustomScriptSection) {
            if (filterScript.isEmpty()) continue;

            QJSValue releaseObject = m_engine->newObject();
            releaseObject.setProperty("title", release->title());
            releaseObject.setProperty("code", release->code());
            releaseObject.setProperty("season", release->season());
            releaseObject.setProperty("status", release->status());
            releaseObject.setProperty("year", release->year());
            releaseObject.setProperty("description", release->description());
            releaseObject.setProperty("releaseType", release->releaseType());
            releaseObject.setProperty("id", release->id());
            releaseObject.setProperty("countOnlineVideos", release->countOnlineVideos());
            releaseObject.setProperty("countTorrents", release->countTorrents());
            releaseObject.setProperty("announce", release->announce());
            releaseObject.setProperty("originalName", release->originalName());
            releaseObject.setProperty("rating", release->rating());
            releaseObject.setProperty("timestamp", release->timestamp());
            releaseObject.setProperty("type", release->type());
            releaseObject.setProperty("series", release->series());
            releaseObject.setProperty("genres", release->genres());
            releaseObject.setProperty("voicers", release->voicers());
            m_engine->globalObject().setProperty("release", releaseObject);

            auto scriptResult = m_engine->evaluate(filterScript);
            if (scriptResult.isError() && !isShowedScriptError) {
                isShowedScriptError = true;
                emit scriptError("Ошибка в скрипте на строке " + scriptResult.property("lineNumber").toString() + " " + scriptResult.toString());
                continue;
            }
            auto isBool = scriptResult.isBool();
            if (!isBool && !isShowedScriptError) {
                isShowedScriptError = true;
                emit scriptError("Скрипт должен возвращать булевое значение!");
                continue;
            }
            if (!scriptResult.toBool()) continue;
        }

        if (!m_customGroups->releaseInFilter(release->id())) continue;

        m_filteredReleases.append(release);
    }

    sortingFilteringReleases(std::move(seenMarks));

    if (m_grouping) {
        QString currentGroup = "";
        foreach (auto filteredRelease, m_filteredReleases) {
            auto groupValue = filteredRelease->groupValue();
            if (currentGroup != groupValue) {
                m_startInGroups.insert(filteredRelease->id());
                currentGroup = groupValue;
            }
        }
    }

    endResetModel();

    setIsHasReleases(m_filteredReleases.count() > 0);
    emit countFilteredReleasesChanged();
}

void ReleasesListModel::selectItem(int id)
{
    m_selectedReleases->insert(id);

    refreshFilteredReleaseById(id);
    emit isHasSelectReleaseChanged();
}

void ReleasesListModel::toggleItem(int id)
{
    if (m_selectedReleases->contains(id)) {
        m_selectedReleases->remove(id);
    } else {
        m_selectedReleases->insert(id);
    }

    refreshFilteredReleaseById(id);
    emit isHasSelectReleaseChanged();
}

void ReleasesListModel::deselectItem(int id)
{
    m_selectedReleases->remove(id);

    refreshFilteredReleaseById(id);
    emit isHasSelectReleaseChanged();
}

void ReleasesListModel::clearSelected()
{
    QSet<int> oldSelectedReleases(*m_selectedReleases);
    m_selectedReleases->clear();

    foreach (auto releaseId, oldSelectedReleases) {
        refreshFilteredReleaseById(releaseId);
    }

    emit isHasSelectReleaseChanged();
}

void ReleasesListModel::refreshSingleItem(int id)
{
    refreshItem(id);
}

void ReleasesListModel::refreshSelectedItems()
{
    if (m_selectedReleases->isEmpty()) return;

    foreach (auto selectedRelease, *m_selectedReleases) {
        refreshItem(selectedRelease);
    }
}

QVariantList ReleasesListModel::selectedIds()
{
    QVariantList result;
    foreach (auto selectedRelease, *m_selectedReleases) {
        result.append(selectedRelease);
    }

    return result;
}

void ReleasesListModel::removeTrimsInStringCollection(QStringList &list)
{
    for (auto i = 0; i < list.count();i++) {
        auto value = list[i];
        list[i] = value.trimmed();
    }
}

bool ReleasesListModel::checkOrCondition(const QStringList &source, const QStringList &target)
{
    auto sourceList = source;
    foreach(QString sourceItem, sourceList) {
        if (target.filter(sourceItem, Qt::CaseInsensitive).count() > 0) return true;
    }

    return false;
}

bool ReleasesListModel::checkAllCondition(const QStringList &source, const QStringList &target)
{
    int counter = 0;
    auto sourceList = source;
    foreach(QString sourceItem, sourceList) {
        if (target.filter(sourceItem, Qt::CaseInsensitive).count() > 0) counter++;
    }

    return counter == source.count();
}

QHash<int, int> &&ReleasesListModel::getAllSeenMarkCount(QHash<int, int>&& result) noexcept
{
    auto keys = m_seenMarkModels->keys();
    foreach (auto key, keys) {
        auto item = m_seenMarkModels->value(key);
        bool mark = std::get<0>(item);
        if (!mark) continue;

        if (!m_videosMap->contains(key)) continue;
        auto video = m_videosMap->value(key);
        auto releaseId = video->releaseId();
        if (!result.contains(releaseId)) {
            result[releaseId] = 1;
        } else {
            result[releaseId] += 1;
        }
    }
    return std::move(result);
}

static bool compareTimeStamp(const FullReleaseModel* first, const FullReleaseModel* second)
{
    return first->timestamp() < second->timestamp();
}

static bool compareTimeStampDescending(const FullReleaseModel* first, const FullReleaseModel* second)
{
    return first->timestamp() > second->timestamp();
}

static bool compareName(const FullReleaseModel* first, const FullReleaseModel* second)
{
    return first->title() < second->title();
}

static bool compareNameDescending(const FullReleaseModel* first, const FullReleaseModel* second)
{
    return first->title() > second->title();
}

static bool compareYear(const FullReleaseModel* first, const FullReleaseModel* second)
{
    return first->year() < second->year();
}

static bool compareYearDescending(const FullReleaseModel* first, const FullReleaseModel* second)
{
    return first->year() > second->year();
}

static bool compareRating(const FullReleaseModel* first, const FullReleaseModel* second)
{
    return first->rating() < second->rating();
}

static bool compareRatingDescending(const FullReleaseModel* first, const FullReleaseModel* second)
{
    return first->rating() > second->rating();
}

static bool compareStatus(const FullReleaseModel* first, const FullReleaseModel* second)
{
    return first->status().toLower() < second->status().toLower();
}

static bool compareStatusDescending(const FullReleaseModel* first, const FullReleaseModel* second)
{
    return first->status().toLower() > second->status().toLower();
}

static bool compareOriginalName(const FullReleaseModel* first, const FullReleaseModel* second)
{
    return first->originalName() < second->originalName();
}

static bool compareOriginalNameDescending(const FullReleaseModel* first, const FullReleaseModel* second)
{
    return first->originalName() > second->originalName();
}

static bool compareSeason(const FullReleaseModel* first, const FullReleaseModel* second)
{
    return first->season().toLower() < second->season().toLower();
}

static bool compareSeasonDescending(const FullReleaseModel* first, const FullReleaseModel* second)
{
    return first->season().toLower() > second->season().toLower();
}

void ReleasesListModel::sortingFilteringReleases(QHash<int, int>&& seenMarks)
{
    auto sortingField = m_sortingField;
    std::function<bool (const FullReleaseModel*, const FullReleaseModel*)> groupValueComparer = [sortingField](const FullReleaseModel* first, const FullReleaseModel* second) {
        if (first->groupValue() == second->groupValue()) {
            //extra sorting inside each groups
            switch (sortingField) {
                case ReleaseTitle:
                    return compareName(first, second);
                case ReleaseRating:
                    return compareRating(first, second);
                case ReleaseOriginalName:
                    return compareOriginalName(first, second);
                case ReleaseSeason:
                    return compareSeason(first, second);
            }
        }
        return first->groupValue() < second->groupValue();
    };

    std::function<bool (const FullReleaseModel*, const FullReleaseModel*)> groupValueDescendingComparer = [sortingField](const FullReleaseModel* first, const FullReleaseModel* second) {
        if (first->groupValue() == second->groupValue()) {
            //extra sorting inside each groups
            switch (sortingField) {
                case ReleaseTitle:
                    return compareNameDescending(first, second);
                case ReleaseRating:
                    return compareRatingDescending(first, second);
                case ReleaseOriginalName:
                    return compareOriginalNameDescending(first, second);
                case ReleaseSeason:
                    return compareSeasonDescending(first, second);
            }
        }

        return first->groupValue() > second->groupValue();
    };

    std::function<bool (const FullReleaseModel*, const FullReleaseModel*)> scheduleComparer = [this](const FullReleaseModel* first, const FullReleaseModel* second) {
        auto firstId = first->id();
        auto firstScheduled = m_scheduleReleases->contains(firstId) ? m_scheduleReleases->value(firstId) : 9;

        auto secondId = second->id();
        auto secondScheduled = m_scheduleReleases->contains(secondId) ? m_scheduleReleases->value(secondId) : 9;

        return firstScheduled < secondScheduled;
    };

    std::function<bool (const FullReleaseModel*, const FullReleaseModel*)> scheduleDescendingComparer = [this](const FullReleaseModel* first, const FullReleaseModel* second) {
        auto firstId = first->id();
        auto firstScheduled = m_scheduleReleases->contains(firstId) ? m_scheduleReleases->value(firstId) : 9;

        auto secondId = second->id();
        auto secondScheduled = m_scheduleReleases->contains(secondId) ? m_scheduleReleases->value(secondId) : 9;

        return firstScheduled > secondScheduled;
    };

    std::function<bool (const FullReleaseModel*, const FullReleaseModel*)> historyComparer = [this](const FullReleaseModel* first, const FullReleaseModel* second) {
        int leftTimestamp = 0;
        int firstId = first->id();
        if (m_historyModels->contains(first->id())) {
            auto historyItem = m_historyModels->value(firstId);
            leftTimestamp = historyItem->timestamp();
        }

        int rightTimestamp = 0;
        int secondId = second->id();
        if (m_historyModels->contains(second->id())) {
            auto historyItem = m_historyModels->value(secondId);
            rightTimestamp = historyItem->timestamp();
        }

        return leftTimestamp < rightTimestamp;
    };

    std::function<bool (const FullReleaseModel*, const FullReleaseModel*)> historyDescendingComparer = [this](const FullReleaseModel* first, const FullReleaseModel* second) {
        int leftTimestamp = 0;
        int firstId = first->id();
        if (m_historyModels->contains(firstId)) {
           auto historyItem = m_historyModels->value(firstId);
           leftTimestamp = historyItem->timestamp();
        }

        int rightTimestamp = 0;
        int secondId = second->id();
        if (m_historyModels->contains(secondId)) {
            auto historyItem = m_historyModels->value(secondId);
            rightTimestamp = historyItem->timestamp();
        }

        return leftTimestamp > rightTimestamp;
    };

    std::function<bool (const FullReleaseModel*, const FullReleaseModel*)> watchHistoryComparer = [this](const FullReleaseModel* first, const FullReleaseModel* second) {
        int leftTimestamp = 0;
        int firstId = first->id();
        if (m_historyModels->contains(first->id())) {
           auto historyItem = m_historyModels->value(firstId);
           leftTimestamp = historyItem->watchTimestamp();
        }

        int rightTimestamp = 0;
        int secondId = second->id();
        if (m_historyModels->contains(second->id())) {
           auto historyItem = m_historyModels->value(secondId);
           rightTimestamp = historyItem->watchTimestamp();
        }

        return leftTimestamp < rightTimestamp;
    };

    std::function<bool (const FullReleaseModel*, const FullReleaseModel*)> watchHistoryDescendingComparer = [this](const FullReleaseModel* first, const FullReleaseModel* second) {
        int leftTimestamp = 0;
        int firstId = first->id();
        if (m_historyModels->contains(first->id())) {
            auto historyItem = m_historyModels->value(firstId);
           leftTimestamp = historyItem->watchTimestamp();
        }

        int rightTimestamp = 0;
        int secondId = second->id();
        if (m_historyModels->contains(second->id())) {
            auto historyItem = m_historyModels->value(secondId);
           rightTimestamp = historyItem->watchTimestamp();
        }

        return leftTimestamp > rightTimestamp;
    };

    std::function<bool (const FullReleaseModel*, const FullReleaseModel*)> favoriteComparer = [this](const FullReleaseModel* first, const FullReleaseModel* second) {

        bool left = m_userFavorites->contains(first->id());

        bool right = m_userFavorites->contains(second->id());

        return left < right;
    };

    std::function<bool (const FullReleaseModel*, const FullReleaseModel*)> favoriteDescendingComparer = [this](const FullReleaseModel* first, const FullReleaseModel* second) {
        bool left = m_userFavorites->contains(first->id());

        bool right = m_userFavorites->contains(second->id());

        return left > right;
    };

    std::function<bool (const FullReleaseModel*, const FullReleaseModel*)> seenComparer = [seenMarks](const FullReleaseModel* first, const FullReleaseModel* second) {
        auto leftCountSeenVideos = seenMarks.contains(first->id()) ? seenMarks.value(first->id()) : 0;
        int leftIndex = leftCountSeenVideos == first->countOnlineVideos() ? 0 : (leftCountSeenVideos > 0 ? 1 : 2);

        auto rightCountSeenVideos = seenMarks.contains(second->id()) ? seenMarks.value(second->id()) : 0;
        int rightIndex = rightCountSeenVideos == second->countOnlineVideos() ? 0 : (rightCountSeenVideos > 0 ? 1 : 2);

        return leftIndex < rightIndex;
    };

    std::function<bool (const FullReleaseModel*, const FullReleaseModel*)> seenDescendingComparer = [seenMarks](const FullReleaseModel* first, const FullReleaseModel* second) {
        auto leftCountSeenVideos = seenMarks.contains(first->id()) ? seenMarks.value(first->id()) : 0;
        int leftIndex = leftCountSeenVideos == first->countOnlineVideos() ? 0 : (leftCountSeenVideos > 0 ? 1 : 2);

        auto rightCountSeenVideos = seenMarks.contains(second->id()) ? seenMarks.value(second->id()) : 0;
        int rightIndex = rightCountSeenVideos == second->countOnlineVideos() ? 0 : (rightCountSeenVideos > 0 ? 1 : 2);

        return leftIndex > rightIndex;
    };

    std::function<bool (const FullReleaseModel*, const FullReleaseModel*)> releaseSeriesComparer = [this](const FullReleaseModel* first, const FullReleaseModel* second) {
        if (m_releaseLinkedSeries == nullptr) return false;

        auto leftOrder = m_releaseLinkedSeries->getSortedOrder(first->id());
        auto rightOrder = m_releaseLinkedSeries->getSortedOrder(second->id());

        return leftOrder < rightOrder;
    };

    std::function<bool (const FullReleaseModel*, const FullReleaseModel*)> releaseSeriesDescendingComparer = [this](const FullReleaseModel* first, const FullReleaseModel* second) {
        if (m_releaseLinkedSeries == nullptr) return false;

        auto leftOrder = m_releaseLinkedSeries->getSortedOrder(first->id());
        auto rightOrder = m_releaseLinkedSeries->getSortedOrder(second->id());

        return leftOrder > rightOrder;
    };

    if (m_grouping) {
        std::sort(m_filteredReleases.begin(), m_filteredReleases.end(), m_sortingDescending ? groupValueDescendingComparer : groupValueComparer);
        return;
    }

    switch (m_sortingField) {
        case ReleaseTimestamp:
            std::sort(m_filteredReleases.begin(), m_filteredReleases.end(), m_sortingDescending ? compareTimeStampDescending : compareTimeStamp);
            break;
        case ReleaseScheduleDay:
            std::sort(m_filteredReleases.begin(), m_filteredReleases.end(), m_sortingDescending ? scheduleDescendingComparer : scheduleComparer);
            break;
        case ReleaseTitle:
            std::sort(m_filteredReleases.begin(), m_filteredReleases.end(), m_sortingDescending ? compareNameDescending : compareName);
            break;
        case ReleaseYear:
            std::sort(m_filteredReleases.begin(), m_filteredReleases.end(), m_sortingDescending ? compareYearDescending : compareYear);
            break;
        case ReleaseRating:
            std::sort(m_filteredReleases.begin(), m_filteredReleases.end(), m_sortingDescending ? compareRatingDescending : compareRating);
            break;
        case ReleaseStatus:
            std::sort(m_filteredReleases.begin(), m_filteredReleases.end(), m_sortingDescending ? compareStatusDescending : compareStatus);
            break;
        case ReleaseOriginalName:
            std::sort(m_filteredReleases.begin(), m_filteredReleases.end(), m_sortingDescending ? compareOriginalNameDescending : compareOriginalName);
            break;
        case ReleaseHistory:
            std::sort(m_filteredReleases.begin(), m_filteredReleases.end(), m_sortingDescending ? historyDescendingComparer : historyComparer);
            break;
        case ReleaseWatchHistory:
            std::sort(m_filteredReleases.begin(), m_filteredReleases.end(), m_sortingDescending ? watchHistoryDescendingComparer : watchHistoryComparer);
            break;
        case ReleaseSeason:
            std::sort(m_filteredReleases.begin(), m_filteredReleases.end(), m_sortingDescending ? compareSeasonDescending : compareSeason);
            break;
        case ReleaseFavoriteMark:
            std::sort(m_filteredReleases.begin(), m_filteredReleases.end(), m_sortingDescending ? favoriteComparer : favoriteDescendingComparer);
            break;
        case ReleaseSeenMark:
            std::sort(m_filteredReleases.begin(), m_filteredReleases.end(), m_sortingDescending ? seenComparer : seenDescendingComparer);
            break;
        case ReleaseSeriesMark: {
            std::sort(m_filteredReleases.begin(), m_filteredReleases.end(), m_sortingDescending ? releaseSeriesComparer : releaseSeriesDescendingComparer);
            break;
        }
    }

}

void ReleasesListModel::refreshFilteredReleaseById(int id)
{
    auto iterator = std::find_if(
        m_filteredReleases.begin(),
        m_filteredReleases.end(),
        [id](FullReleaseModel* item)
        {
            return item->id() == id;
        }
    );

    if(iterator == m_filteredReleases.end()) return;

    int itemIndex = m_filteredReleases.indexOf(*iterator);
    emit dataChanged(index(itemIndex), index(itemIndex));
}

QString ReleasesListModel::getGroupByRelease(const FullReleaseModel* model, const QHash<int, int>& seens)
{
    if (m_sortingField == ReleaseTimestamp || m_sortingField == ReleaseHistory || m_sortingField == ReleaseWatchHistory) return "";

    if (m_sortingField == ReleaseYear) return model->year();
    if (m_sortingField == ReleaseRating) return getGroupForRating(model->rating());
    if (m_sortingField == ReleaseStatus) return model->status();
    if (m_sortingField == ReleaseSeason) return model->season().toLower();

    if (m_sortingField == ReleaseTitle) {
        auto title = model->title().toUpper();
        if (title.isEmpty()) return "";

        auto firstChar = title[0] + " символ";
        return QString(firstChar);
    }

    if (m_sortingField == ReleaseOriginalName) {
        auto title = model->originalName().toUpper();
        if (title.isEmpty()) return "";

        auto firstChar = title[0] + " символ";
        return QString(firstChar);
    }

    auto releaseId = model->id();

    if (m_sortingField == ReleaseScheduleDay) {

        auto isSchedule = m_scheduleReleases->contains(releaseId);
        auto dayInSchedule = m_scheduleReleases->value(releaseId);
        return isSchedule ? QString::number(dayInSchedule) + "." + getScheduleDay(dayInSchedule) : "";
    }

    if (m_sortingField == ReleaseFavoriteMark) return m_userFavorites->contains(releaseId) ? "В избранном" : "Не в избранном";
    if (m_sortingField == ReleaseSeriesMark) return m_releaseLinkedSeries != nullptr ? m_releaseLinkedSeries->getFranchiseTitle(releaseId) : "";
    if (m_sortingField == ReleaseSeenMark) {
        auto countSeens = seens.contains(releaseId) ? seens.value(releaseId) : 0;
        if (countSeens == model->countOnlineVideos()) return "Просмотрено";
        return countSeens > 0 ? "Просматривается" : "Не просмотрено";
    }

    return "";
}

QString ReleasesListModel::getGroupForRating(int rating)
{
    if (rating < 100) return "0+";
    if (rating < 1000) return "100+";
    if (rating < 10000) return "1000+";
    if (rating < 20000) return "10000+";
    if (rating < 30000) return "20000+";
    return "30000+";
}
