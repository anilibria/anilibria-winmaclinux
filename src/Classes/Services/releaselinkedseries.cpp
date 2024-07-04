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
#include <QDebug>
#include <QVector>
#include <QRegularExpression>
#include <QtConcurrent>
#include <QFuture>
#include <QMap>
#include <QSet>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "releaselinkedseries.h"
#include "globalconstants.h"

ReleaseLinkedSeries::ReleaseLinkedSeries(QObject *parent) : QAbstractListModel(parent)
{
    createCacheFileIfNotExists();

    auto watcher = m_cacheUpdateWatcher.get();
    connect(watcher, &QFutureWatcher<bool>::finished, this, &ReleaseLinkedSeries::cacheUpdated);
}

int ReleaseLinkedSeries::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_filtering ? m_filteredSeries.size() : m_series.size();
}

QVariant ReleaseLinkedSeries::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto series = m_filtering ? m_filteredSeries : m_series;

    auto elementIndex = index.row();
    auto element = series.at(elementIndex);

    switch (role) {
        case CountReleasesRole: {
            return QVariant(element->countReleases());
        }
        case FirstThreeNamesRole: {
            if (element->titles()->count() > 2) {
                QStringList titleList;
                int iterator = 0;
                foreach (auto title, *element->titles()) {
                    if (iterator == 3) break;

                    titleList.append(title);
                }
                return QVariant(titleList.join(", "));
            }
            return element->titles()->join(", ");
        }
        case ReleaseIds: {
            return QVariant(*series.at(index.row())->releaseIds());
        }
        case FirstPosterRole: {
            return QVariant(element->firstPoster());
        }
        case SecondPosterRole: {
            return QVariant(element->secondPoster());
        }
        case ThirdPosterRole: {
            return QVariant(element->thirdPoster());
        }
        case OtherReleasesRole: {
            auto count = element->countReleases();

            QString other = "";
            if (count > 3) other += " + еще " + QString::number(count - 3) + " релиза";

            return QVariant(other);
        }
        case GenresRole: {
            return QVariant(element->genresAsString());
        }
        case CountInFavoritesRole: {
            auto ids = element->releaseIds();
            auto countReleases = element->countReleases();
            int countInFavorites = 0;

            foreach (auto id, *ids) {
                if (m_userFavorites->contains(id.toInt())) {
                    countInFavorites++;
                }
            }

            if (countInFavorites == countReleases) return QVariant("Все в избранном");
            if (countInFavorites <= countReleases && countInFavorites > 0) return QVariant(QString::number(countInFavorites) + " в избранном");
            return QVariant("Не добавлено в избранное");
        }
        case IdentifierRole: {
            return QVariant(m_series.indexOf(element));
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ReleaseLinkedSeries::roleNames() const
{
    return {
        {
            CountReleasesRole,
            "countReleases"
        },
        {
            FirstThreeNamesRole,
            "firstThreeNamesRole"
        },
        {
            ReleaseIds,
            "releaseIds"
        },
        {
            FirstPosterRole,
            "firstPoster"
        },
        {
            SecondPosterRole,
            "secondPoster"
        },
        {
            ThirdPosterRole,
            "thirdPoster"
        },
        {
            OtherReleasesRole,
            "otherReleases"
        },
        {
            GenresRole,
            "genres"
        },
        {
            CountInFavoritesRole,
            "countInFavorites"
        },
        {
            IdentifierRole,
            "identifier"
        }
    };
}

void ReleaseLinkedSeries::setup(QSharedPointer<QList<FullReleaseModel *> > releases, QVector<int>* userFavorites, QList<ApiTorrentModel*>* torrents)
{
    m_releases = releases;
    m_userFavorites = userFavorites;
    m_torrents = torrents;
}

void ReleaseLinkedSeries::setNameFilter(const QString& nameFilter) noexcept
{
    if (nameFilter == m_nameFilter) return;

    m_nameFilter = nameFilter;
    emit nameFilterChanged();
}

void ReleaseLinkedSeries::setSortingField(int sortingField) noexcept
{
    if (m_sortingField == sortingField) return;

    m_sortingField = sortingField;
    emit sortingFieldChanged();

    sortNonFiltered();
    filterSeries();
}

void ReleaseLinkedSeries::setSortingDirection(bool sortingDirection) noexcept
{
    if (m_sortingDirection == sortingDirection) return;

    m_sortingDirection = sortingDirection;
    emit sortingDirectionChanged();

    sortNonFiltered();
    filterSeries();
}

void ReleaseLinkedSeries::setApiv2host(const QString &apiv2host) noexcept
{
    if (m_apiv2host == apiv2host) return;

    m_apiv2host = apiv2host;
    emit apiv2hostChanged();

    refreshSeries();
}

QSharedPointer<QList<int>> ReleaseLinkedSeries::getAllLinkedReleases() const noexcept
{
    auto allReleases = QSharedPointer<QList<int>>(new QList<int>());

    foreach (auto serie, m_series) {
        auto ids = serie->releaseIds();
        foreach (auto id, *ids) {
            allReleases->append(id.toInt());
        }
    }

    return allReleases;
}

QList<QList<int>> ReleaseLinkedSeries::getFullLinkedReleases() const noexcept
{
    QList<QList<int>> allReleases;

    foreach (auto serie, m_series) {
        auto ids = serie->releaseIds();
        QList<int> innerIds;
        foreach (auto id, *ids) {
            innerIds.append(id.toInt());
        }
        allReleases.append(innerIds);
    }

    return allReleases;
}

int ReleaseLinkedSeries::getSortedOrder(int id) const noexcept
{
    int iterator = 0;
    foreach (auto item, m_series) {
        if (!item->releaseIds()->contains(id)) {
            iterator += 100;
            continue;
        }

        return iterator + item->releaseIds()->indexOf(id);
    }

    return 0;
}

void ReleaseLinkedSeries::fillReleaseSeries(QList<FullReleaseModel *> *list, const int id) noexcept
{
    auto iterator = std::find_if(
        m_series.cbegin(),
        m_series.cend(),
        [id] (ReleaseSeriesModel* model) {
            return model->releaseIds()->contains(id);
        }
    );

    if (iterator == m_series.cend()) return;

    auto item = *iterator;
    auto idsCollection = item->releaseIds();

    for (auto i = 0; i < idsCollection->count(); i++) {
        list->append(nullptr);
    }

    foreach (auto release, *m_releases) {
        auto releaseId = release->id();
        if (idsCollection->contains(releaseId)) {
            auto releaseIndex = idsCollection->indexOf(releaseId);
            (*list)[releaseIndex] = release;
        }
    }
}

int ReleaseLinkedSeries::getNextLinkedRelease(const int currentRelease)
{
    foreach (auto item, m_series) {
        if (!item->releaseIds()->contains(currentRelease)) continue;

        auto index = item->releaseIds()->indexOf(currentRelease);
        return index < item->releaseIds()->count() - 1 ? item->releaseIds()->value(index + 1).toInt() : 0;
    }

    return 0;
}

void ReleaseLinkedSeries::refreshSeries()
{
    beginResetModel();

    loadSeries();

    refreshDataFromReleases();

    endResetModel();

    filterSeries();
}

bool ReleaseLinkedSeries::isReleaseInSeries(int id)
{
    auto series = std::find_if(m_series.begin(), m_series.end(), [id](ReleaseSeriesModel* model) {
        return model->releaseIds()->contains(id);
    });

    return series != m_series.end();
}

void ReleaseLinkedSeries::filterSeries()
{
    beginResetModel();

    m_filtering = true;
    m_filteredSeries.clear();

    auto nameFilter = m_nameFilter.toLower();

    std::copy_if(
        m_series.begin(),
        m_series.end(),
        std::back_inserter(m_filteredSeries),
        [nameFilter] (ReleaseSeriesModel* model) {
            if (model->genresAsString().toLower().contains(nameFilter)) return true;

            auto titles = model->titles();
            auto title = std::find_if(
                titles->begin(),
                titles->end(),
                [nameFilter](const QVariant& title) {
                    return title.toString().toLower().contains(nameFilter);
                }
            );

            auto isTitles = title != titles->end();
            return isTitles;
        }
    );

    endResetModel();
    emit countGroupsChanged();
}

void ReleaseLinkedSeries::clearFilters()
{
    setNameFilter("");
    m_filtering = false;

    filterSeries();
    emit countGroupsChanged();
}

void ReleaseLinkedSeries::selectByIndex(int index)
{
    if (index < 0 || index >= m_series.count()) return;

    m_isCardShowed = true;
    m_selectedIndex = index;

    QList<FullReleaseModel*> selectedReleases;

    auto series = m_series.at(index);

    for (auto i = 0; i < series->countReleases(); i++) {
        selectedReleases.append(nullptr);
    }

    QSet<int> inFavorites;

    auto ids = series->releaseIds();
    foreach (auto release, *m_releases) {
        auto releaseId = release->id();
        if (ids->contains(releaseId)) {
            auto releaseIndex = ids->indexOf(releaseId);
            selectedReleases[releaseIndex] = release;
            if (m_userFavorites->contains(releaseId)) inFavorites.insert(releaseId);
        }
    }

    m_releaseSeriesCardList->setup(selectedReleases, inFavorites);

    emit isCardShowedChanged();
}

void ReleaseLinkedSeries::closeCard()
{
    m_isCardShowed = false;
    emit isCardShowedChanged();

    m_releaseSeriesCardList->clear();
}

void ReleaseLinkedSeries::refreshCard()
{
    m_releaseSeriesCardList->clear();

    selectByIndex(m_selectedIndex);
}

QString ReleaseLinkedSeries::getSeriesCachePath() const noexcept
{
    if (IsPortable) {
        return QDir::currentPath() + "/releaseseries.cache";
    } else {
        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/releaseseries.cache";
    }
}

QString ReleaseLinkedSeries::getReleasesCachePath() const noexcept
{
    if (IsPortable) {
        return QDir::currentPath() + "/releases.cache";
    } else {
        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/releases.cache";
    }
}

void ReleaseLinkedSeries::loadSeries()
{
    QFile seriesFile(getSeriesCachePath());
    if (!seriesFile.open(QFile::ReadOnly | QFile::Text)) {
        qInfo() << "Can't read series file !";
        return;
    }
    auto json = seriesFile.readAll();
    seriesFile.close();

    auto document = QJsonDocument::fromJson(json);
    auto jsonArray = document.array();

    m_series.clear();
    foreach (auto item, jsonArray) {
        auto seriaModel = new ReleaseSeriesModel();
        seriaModel->readFromJson(item.toObject());
        seriaModel->setPosterHost(m_apiv2host);

        m_series.append(seriaModel);
    }

    sortNonFiltered();
}

void ReleaseLinkedSeries::createCacheFileIfNotExists() const noexcept
{
    auto path = getSeriesCachePath();
    if (!QFile::exists(path)) {
        QFile createReleasesCacheFile(path);
        createReleasesCacheFile.open(QFile::WriteOnly | QFile::Text);
        createReleasesCacheFile.write("[]");
        createReleasesCacheFile.close();
    }
}

void ReleaseLinkedSeries::processReleasesFromDescription(const QString& description, const QMap<QString, FullReleaseModel*>& releases, int currentRelease, const QString currentReleaseTitle, const QString& poster, const QString& genres) noexcept
{
    QString startToken = "Порядок просмотра";
    int watchOrderIndex = description.indexOf(startToken);
    if (watchOrderIndex == -1) return;

    auto cuttedDescription = description.mid(watchOrderIndex + startToken.length());
    auto parts = cuttedDescription.split("#").mid(1);

    static QRegularExpression linkRegexp(R"(\/release\/(.*)\.html)");

    //if already have series that have greater releases it means that don't need processing
    auto seriesItem = std::find_if(
        m_series.begin(),
        m_series.end(),
        [currentRelease] (ReleaseSeriesModel* model) {
            return model->releaseIds()->contains(currentRelease);
        }
    );
    if (seriesItem != m_series.end() && (*seriesItem)->countReleases() >= parts.length()) return;
    // if already have series with less items then remove it
    if (seriesItem != m_series.end()) m_series.removeOne(*seriesItem);

    auto series = new ReleaseSeriesModel();

    foreach (auto part, parts) {
        auto partString = part;

        auto match = linkRegexp.match(partString);

        if (match.hasMatch()) {
            auto link = match.captured(1);
            if (releases.contains(link)) {
                auto release = releases[link];
                if (series->appendReleaseId(release->id())) {
                    series->appendPoster(release->poster());
                    series->appendTitle(release->title());
                    foreach (auto genre, release->genres().split(",")) {
                        series->appendGenre(genre.trimmed());
                    }
                }
            }
        } else {
            if (series->appendReleaseId(currentRelease)) {
                series->appendPoster(poster);
                series->appendTitle(currentReleaseTitle);
                foreach (auto genre, genres.split(",")) {
                    series->appendGenre(genre.trimmed());
                }
            }
        }
    }

    // additional check if it last release with all links to another
    if (!series->releaseIds()->isEmpty()) {
        auto firstRelease = series->releaseIds()->first();
        auto anotherSeriesItem = std::find_if(
            m_series.begin(),
            m_series.end(),
            [firstRelease] (ReleaseSeriesModel* model) {
                return model->releaseIds()->contains(firstRelease);
            }
        );
        if (anotherSeriesItem != m_series.end()) m_series.removeOne(*anotherSeriesItem);
    }

    // recalculate and save series
    series->recalculateCountReleases();

    if (series->countReleases() >= 2) m_series.append(series);
}

void ReleaseLinkedSeries::sortNonFiltered()
{
    auto sortingField = m_sortingField;
    auto sortingDirection = m_sortingDirection;
    std::sort(
        m_series.begin(),
        m_series.end(),
        [sortingField, sortingDirection](ReleaseSeriesModel* left, ReleaseSeriesModel* right) {
            if (sortingField == 1) {
                return  sortingDirection ? left->titleAsString() > right->titleAsString() : left->titleAsString() < right->titleAsString();
            }

            if (sortingField == 2) {
                return sortingDirection ? left->genresAsString() > right->genresAsString() : left->genresAsString() < right->genresAsString();
            }

            if (sortingField == 3) {
                return sortingDirection ? left->sumOfRatings() > right->sumOfRatings() : left->sumOfRatings() < right->sumOfRatings();
            }

            if (sortingField == 4) {
                return sortingDirection ? left->sumOfSeeds() > right->sumOfSeeds() : left->sumOfSeeds() < right->sumOfSeeds();
            }

            if (sortingField == 5) {
                return sortingDirection ? left->seasons() > right->seasons() : left->seasons() < right->seasons();
            }

            if (sortingField == 6) {
                return sortingDirection ? left->years() > right->years() : left->years() < right->years();
            }

            return  sortingDirection ? left->countReleases() > right->countReleases() : left->countReleases() < right->countReleases();
        }
    );
}

void ReleaseLinkedSeries::refreshDataFromReleases()
{
    QMap<int, FullReleaseModel*> m_releasesMap;
    foreach (auto release, *m_releases){
        m_releasesMap.insert(release->id(), release);
    }

    QStringList years;
    QStringList seasons;

    foreach (auto series, m_series){
        int rating = 0;
        int seeds = 0;
        years.clear();
        seasons.clear();

        auto ids = series->releaseIds();
        foreach (auto id, *ids) {
            auto identifier = id.toInt();
            if (!m_releasesMap.contains(identifier)) continue;
            auto release = m_releasesMap[identifier];

            rating += release->rating();
            if (!years.contains(release->year())) years.append(release->year());
            if (!seasons.contains(release->season())) seasons.append(release->season());
            seeds += getSeeders(release);

            foreach (auto genre, release->genres().split(",")) {
                series->appendGenre(genre.trimmed());
            }
        }

        series->setSumOfRatings(rating);
        series->setSumOfSeeds(seeds);
        std::sort(
            years.begin(),
            years.end(),
            [](const QString& left, const QString& right) {
                return left < right;
            }
        );
        std::sort(
            seasons.begin(),
            seasons.end(),
            [](const QString& left, const QString& right) {
                return left < right;
            }
        );
        auto yearsString = years.join(", ");
        auto seasonsString = seasons.join(", ");
        series->setYears(yearsString);
        series->setSeasons(seasonsString);
    }
}

int ReleaseLinkedSeries::getSeeders(FullReleaseModel *release)
{
    auto result = 0;
    foreach (auto item, *m_torrents) {
        if (item->releaseId() != release->id()) continue;

        result += item->seeders();
    }

    return result;
}

void ReleaseLinkedSeries::cacheUpdated()
{
    if (!m_cacheUpdateWatcher->result()) return;

    refreshDataFromReleases();

    sortNonFiltered();

    filterSeries();
}
