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

ReleasesListModel::ReleasesListModel(QList<FullReleaseModel*>* releases, QObject *parent) : QAbstractListModel(parent)
{
    m_releases = releases;
}

int ReleasesListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_filteredReleases->size();
}

QVariant ReleasesListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto release = m_filteredReleases->at(index.row());

    switch (role) {
        case ReleaseIdRole: {
            return QVariant(release->id());
        }
        case TitleRole: {
            return QVariant(release->title());
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
}

void ReleasesListModel::setTypeFilter(const QString &typeFilter) noexcept
{
    if (m_typeFilter == typeFilter) return;

    m_typeFilter = typeFilter;
    emit typeFilterChanged();
}

void ReleasesListModel::setYearsFilter(const QString &yearsFilter) noexcept
{
    if (m_yearsFilter == yearsFilter) return;

    m_yearsFilter = yearsFilter;
    emit yearsFilterChanged();
}

void ReleasesListModel::setStatusesFilter(const QString &statusesFilter) noexcept
{
    if (m_statusesFilter == statusesFilter) return;

    m_statusesFilter = statusesFilter;
    emit statusesFilterChanged();
}

void ReleasesListModel::setSeasonesFilter(const QString &seasonesFilter) noexcept
{
    if (m_seasonesFilter == seasonesFilter) return;

    m_seasonesFilter = seasonesFilter;
    emit seasonesFilterChanged();
}

void ReleasesListModel::setGenresFilter(const QString &genresFilter) noexcept
{
    if (m_genresFilter == genresFilter) return;

    m_genresFilter = genresFilter;
    emit genresFilterChanged();
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
}

void ReleasesListModel::setSeenMarkFilter(int seenMarkFilter) noexcept
{
    if (m_seenMarkFilter == seenMarkFilter) return;

    m_seenMarkFilter = seenMarkFilter;
    emit seenMarkFilterChanged();
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

void ReleasesListModel::refresh()
{
    beginResetModel();

    m_filteredReleases->clear();

    QHash<int, int> hash1;
    auto seenMarks = getAllSeenMarkCount(std::move(hash1));

    foreach (auto release, *m_releases) {
        if (m_hiddenReleases->contains(release->id()) && m_section != HiddenReleasesSection) continue;
        if (!m_titleFilter.isEmpty()) {
            auto filteredTitle = m_titleFilter.toLower().replace("ё", "е").trimmed();
            auto inTitle = release->title().toLower().replace("ё", "е").contains(filteredTitle);
            auto inOriginalTitle = release->originalName().toLower().replace("ё", "е").contains(filteredTitle);
            if (!(inTitle || inOriginalTitle)) continue;
        }
        if (!m_descriptionFilter.isEmpty() && !release->description().toLower().contains(m_descriptionFilter.toLower())) continue;
        if (!m_typeFilter.isEmpty() && !release->type().toLower().contains(m_typeFilter.toLower())) continue;

        //years
        if (!m_yearsFilter.isEmpty()) {
            QStringList yearsList = m_yearsFilter.split(",");
            removeTrimsInStringCollection(yearsList);
            int year = release->year().toInt();
            QStringList singleYear;
            singleYear.append(QString::number(year));

            if (!checkOrCondition(yearsList, singleYear)) continue;
        }

        //statuses
        if (!m_statusesFilter.isEmpty()) {
            QStringList statusesList = m_statusesFilter.split(",");
            removeTrimsInStringCollection(statusesList);
            QStringList singleStatus;
            singleStatus.append(release->status());

            if (!checkOrCondition(statusesList, singleStatus)) continue;
        }

        //seasons
        if (!m_seasonesFilter.isEmpty()) {
            QStringList seasonesList = m_seasonesFilter.split(",");
            removeTrimsInStringCollection(seasonesList);
            auto season = release->season();
            QStringList singleSeason;
            singleSeason.append(season);

            if (!checkOrCondition(seasonesList, singleSeason)) continue;
        }

        //genres
        if (!m_genresFilter.isEmpty()) {
            QStringList genresList = m_genresFilter.split(",");
            removeTrimsInStringCollection(genresList);
            QStringList releaseGenresList = release->genres().split(",");
            if (m_genresFilterOr) {
                if (!checkAllCondition(genresList, releaseGenresList)) continue;
            } else {
                if (!checkOrCondition(genresList, releaseGenresList)) continue;
            }
        }

        //voices
        if (!m_voicesFilter.isEmpty()) {
            QStringList voicesList = m_voicesFilter.split(",");
            QStringList releaseVoicesList = release->voicers().split(",");
            if (m_voicesFilterOr) {
                if (!checkAllCondition(voicesList, releaseVoicesList)) continue;
            } else {
                if (!checkOrCondition(voicesList, releaseVoicesList)) continue;
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

        //favorite mark
        if (m_favoriteMarkFilter == 1 && !m_userFavorites->contains(release->id())) continue;
        if (m_favoriteMarkFilter == 2 && m_userFavorites->contains(release->id())) continue;

        //seen mark
        auto countVideos = seenMarks.contains(release->id()) ? seenMarks.value(release->id()) : 0;
        int seenState = countVideos == release->countOnlineVideos() ? 0 : (countVideos > 0 ? 1 : 2);
        if (m_seenMarkFilter == 1 && !(seenState == 0)) continue;
        if (m_seenMarkFilter == 2 && !(seenState == 1)) continue;
        if (m_seenMarkFilter == 3 && !(seenState == 2)) continue;

        //favorites section
        if (m_section == FavoriteSection && !m_userFavorites->contains(release->id())) continue;

        if (m_section == ScheduleSection && !m_scheduleReleases->contains(release->id())) continue;

        if (m_section == NewReleasesSection && !m_changesModel->newReleases()->contains(release->id())) continue;

        if (m_section == NewOnlineSeriesSection && !m_changesModel->newOnlineSeries()->contains(release->id())) continue;

        if (m_section == NewTorrentsSection && !m_changesModel->newTorrents()->contains(release->id())) continue;

        if (m_section == NewTorrentSeriesSection && !m_changesModel->newTorrentSeries()->contains(release->id())) continue;

        //TODO: add getting from options
        auto notificationForFavorites = false;//m_UserSettingsModel->notificationForFavorites();
        bool isInFavorites = m_userFavorites->contains(release->id());

        if ((m_section == NewOnlineSeriesSection ||
           m_section == NewTorrentsSection ||
           m_section == NewTorrentSeriesSection) && notificationForFavorites && !isInFavorites) continue;

        if (m_section == HistorySection && !(m_historyModels->contains(release->id()) && m_historyModels->value(release->id())->timestamp() > 0)) continue;

        if (m_section == WatchHistorySection && !(m_historyModels->contains(release->id()) && m_historyModels->value(release->id())->watchTimestamp() > 0)) continue;

        auto countReleaseSeenVideos = seenMarks.contains(release->id()) ? seenMarks.value(release->id()) : 0;
        auto isAllSeens = countReleaseSeenVideos == release->countOnlineVideos() && release->countOnlineVideos() > 0;
        if (m_section == SeenHistorySection && !isAllSeens) continue;

        if (m_section == SeeningHistorySection && !(countReleaseSeenVideos > 0 && !isAllSeens)) continue;

        if (m_section == NotSeeningHistorySection && !(countReleaseSeenVideos == 0)) continue;

        if (m_section == HiddenReleasesSection && !m_hiddenReleases->contains(release->id())) continue;

        m_filteredReleases->append(release);
    }

    sortingFilteringReleases(std::move(seenMarks));

    endResetModel();
}

void ReleasesListModel::removeTrimsInStringCollection(const QStringList &list)
{
    auto stringList = list;
    QMutableStringListIterator iterator(stringList);
    while (iterator.hasNext()) {
        QString value = iterator.next();
        iterator.setValue(value.trimmed());
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
    QHashIterator<QString, bool> iterator(*m_seenMarkModels);
    while(iterator.hasNext()) {
        iterator.next();

        QString key = iterator.key();
        auto keyParts = key.split(".");
        auto releaseId = keyParts.first().toInt();
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
    return first->status() < second->status();
}

static bool compareStatusDescending(const FullReleaseModel* first, const FullReleaseModel* second)
{
    return first->status() > second->status();
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
    return first->season() < second->season();
}

static bool compareSeasonDescending(const FullReleaseModel* first, const FullReleaseModel* second)
{
    return first->season() > second->season();
}

void ReleasesListModel::sortingFilteringReleases(QHash<int, int>&& seenMarks)
{
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

    switch (m_sortingField) {
        case ReleaseTimestamp:
            std::sort(m_filteredReleases->begin(), m_filteredReleases->end(), m_sortingDescending ? compareTimeStampDescending : compareTimeStamp);
            break;
        case ReleaseScheduleDay:
            std::sort(m_filteredReleases->begin(), m_filteredReleases->end(), m_sortingDescending ? scheduleDescendingComparer : scheduleComparer);
            break;
        case ReleaseTitle:
            std::sort(m_filteredReleases->begin(), m_filteredReleases->end(), m_sortingDescending ? compareNameDescending : compareName);
            break;
        case ReleaseYear:
            std::sort(m_filteredReleases->begin(), m_filteredReleases->end(), m_sortingDescending ? compareYearDescending : compareYear);
            break;
        case ReleaseRating:
            std::sort(m_filteredReleases->begin(), m_filteredReleases->end(), m_sortingDescending ? compareRatingDescending : compareRating);
            break;
        case ReleaseStatus:
            std::sort(m_filteredReleases->begin(), m_filteredReleases->end(), m_sortingDescending ? compareStatusDescending : compareStatus);
            break;
        case ReleaseOriginalName:
            std::sort(m_filteredReleases->begin(), m_filteredReleases->end(), m_sortingDescending ? compareOriginalNameDescending : compareOriginalName);
            break;
        case ReleaseHistory:
            std::sort(m_filteredReleases->begin(), m_filteredReleases->end(), m_sortingDescending ? historyDescendingComparer : historyComparer);
            break;
        case ReleaseWatchHistory:
            std::sort(m_filteredReleases->begin(), m_filteredReleases->end(), m_sortingDescending ? watchHistoryDescendingComparer : watchHistoryComparer);
            break;
        case ReleaseSeason:
            std::sort(m_filteredReleases->begin(), m_filteredReleases->end(), m_sortingDescending ? compareSeasonDescending : compareSeason);
            break;
        case ReleaseFavoriteMark:
            std::sort(m_filteredReleases->begin(), m_filteredReleases->end(), m_sortingDescending ? favoriteComparer : favoriteDescendingComparer);
            break;
        case ReleaseSeenMark:
            std::sort(m_filteredReleases->begin(), m_filteredReleases->end(), m_sortingDescending ? seenComparer : seenDescendingComparer);
            break;
    }

}
