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
#include <QRegExp>
#include "releaselinkedseries.h"
#include "globalconstants.h"

ReleaseLinkedSeries::ReleaseLinkedSeries(QObject *parent) : QAbstractListModel(parent),
    m_nameFilter(""),
    m_series(new QVector<ReleaseSeriesModel*>()),
    m_filteredSeries(new QVector<ReleaseSeriesModel*>())
{
    createCacheFileIfNotExists();
    loadSeries();
}

int ReleaseLinkedSeries::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_filtering ? m_filteredSeries->size() : m_series->size();
}

QVariant ReleaseLinkedSeries::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto series = m_filtering ? *m_filteredSeries : *m_series;

    auto element = series.at(index.row());

    switch (role) {
        case CountReleasesRole: {
            return QVariant(element->countReleases());
        }
        case FirstNameRole: {
            return QVariant(element->titles()->first().toString());
        }
        case SecondNameRole: {
            return QVariant(element->titles()->at(1).toString());
        }
        case ThirdNameRole: {
            if (element->titles()->length() > 2) {
                return QVariant(QVariant(element->titles()->at(2).toString()));
            } else {
                return QVariant("");
            }
        }
        case ReleaseIds: {
            return QVariant(*series.at(index.row())->releaseIds());
        }
        case Posters: {
            return QVariant(*series.at(index.row())->posters());
        }
        case FirstPosterRole: {
            return QVariant(element->posters()->first().toString());
        }
        case SecondPosterRole: {
            return QVariant(element->posters()->at(1).toString());
        }
        case ThirdPosterRole: {
            if (element->posters()->length() > 2) {
                return QVariant(QVariant(element->posters()->at(2).toString()));
            } else {
                return QVariant("");
            }
        }
        case OtherReleasesRole: {
            auto count = series.at(index.row())->countReleases();

            QString other = "";
            if (count > 3) other += " + еще " + QString::number(count - 3) + " релиза";

            return QVariant(other);
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
            FirstNameRole,
            "firstName"
        },
        {
            SecondNameRole,
            "secondName"
        },
        {
            ThirdNameRole,
            "thirdName"
        },
        {
            ReleaseIds,
            "releaseIds"
        },
        {
            Posters,
            "posters"
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

    };
}

void ReleaseLinkedSeries::setNameFilter(const QString& nameFilter) noexcept
{
    if (nameFilter == m_nameFilter) return;

    m_nameFilter = nameFilter;
    emit nameFilterChanged();
}

QSharedPointer<QList<int>> ReleaseLinkedSeries::getAllLinkedReleases() const noexcept
{
    auto allReleases = QSharedPointer<QList<int>>(new QList<int>());

    foreach (auto serie, *m_series) {
        auto ids = serie->releaseIds();
        foreach (auto id, *ids) {
            allReleases->append(id.toInt());
        }
    }

    return allReleases;
}

int ReleaseLinkedSeries::getSortedOrder(int id) const noexcept
{
    int iterator = 0;
    foreach (auto item, *m_series) {
        if (!item->releaseIds()->contains(id)) {
            iterator += 100;
            continue;
        }

        return iterator + item->releaseIds()->indexOf(id);
    }

    return 0;
}

void ReleaseLinkedSeries::refreshSeries()
{
    QFile releasesCacheFile(getReleasesCachePath());
    if (!releasesCacheFile.open(QFile::ReadOnly | QFile::Text)) {
        qInfo() << "Can't read release cache file !";
        return;
    }

    QString releasesJson = releasesCacheFile.readAll();
    releasesCacheFile.close();
    auto releasesArray = QJsonDocument::fromJson(releasesJson.toUtf8()).array();
    QMap<QString, FullReleaseModel*> releases;

    foreach (auto release, releasesArray) {
        auto jsonRelease = new FullReleaseModel();
        jsonRelease->readFromJson(release);

        releases.insert(jsonRelease->code(), jsonRelease);
    }

    m_series->clear();

    foreach (auto release, releases) {
        auto description = release->description();
        processReleasesFromDescription(description, releases, release->id(), release->title(), release->poster());
    }

    saveSeries();

    filterSeries(); // update data in UI
}

bool ReleaseLinkedSeries::isReleaseInSeries(int id)
{
    auto series = std::find_if(m_series->begin(), m_series->end(), [id](ReleaseSeriesModel* model) {
        return model->releaseIds()->contains(id);
    });

    return series != m_series->end();
}

void ReleaseLinkedSeries::filterSeries()
{
    beginResetModel();

    m_filtering = true;
    m_filteredSeries->clear();

    auto nameFilter = m_nameFilter;

    std::copy_if(
        m_series->begin(),
        m_series->end(),
        std::back_inserter(*m_filteredSeries),
        [nameFilter] (ReleaseSeriesModel* model) {
            auto titles = model->titles();
            auto title = std::find_if(
                titles->begin(),
                titles->end(),
                [nameFilter](const QVariant& title) {
                    return title.toString().toLower().contains(nameFilter.toLower());
                }
            );

            auto isTitles = title != titles->end();
            return isTitles;
        }
    );

    endResetModel();
}

void ReleaseLinkedSeries::clearFilters()
{
    setNameFilter("");
    m_filtering = false;

    filterSeries();
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

    m_series->clear();
    foreach (auto item, jsonArray) {
        auto seriaModel = new ReleaseSeriesModel();
        seriaModel->readFromJson(item.toObject());

        m_series->append(seriaModel);
    }
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

void ReleaseLinkedSeries::processReleasesFromDescription(const QString& description, const QMap<QString, FullReleaseModel*>& releases, int currentRelease, const QString currentReleaseTitle, const QString& poster) noexcept
{
    QString startToken = "Порядок просмотра:";
    int watchOrderIndex = description.indexOf(startToken);
    if (watchOrderIndex == -1) return;

    auto cuttedDescription = description.midRef(watchOrderIndex + startToken.length());
    auto parts = cuttedDescription.split("#").mid(1);

    QRegExp linkRegexp("(https|http)\\:\\/\\/(www\\.anilibria|anilibria)\\.tv\\/release\\/(.*)\\.html");

    //if already have series that have greater releases it means that don't need processing
    auto seriesItem = std::find_if(
        m_series->begin(),
        m_series->end(),
        [currentRelease] (ReleaseSeriesModel* model) {
            return model->releaseIds()->contains(currentRelease);
        }
    );
    if (seriesItem != m_series->end() && (*seriesItem)->countReleases() >= parts.length()) return;
    // if already have series with less items then remove it
    if (seriesItem != m_series->end()) m_series->removeOne(*seriesItem);

    auto series = new ReleaseSeriesModel();

    foreach (auto part, parts) {
        auto partString = part.toString();

        if (linkRegexp.indexIn(partString, 0) > -1) {
            auto link = linkRegexp.cap(3);
            if (releases.contains(link)) {
                auto release = releases[link];
                if (series->appendReleaseId(release->id())) {
                    series->appendPoster(release->poster());
                    series->appendTitle(release->title());
                }
            }
        } else {
            if (series->appendReleaseId(currentRelease)) {
                series->appendPoster(poster);
                series->appendTitle(currentReleaseTitle);
            }
        }
    }

    // additional check if it last release with all links to another
    if (!series->releaseIds()->isEmpty()) {
        auto firstRelease = series->releaseIds()->first();
        auto anotherSeriesItem = std::find_if(
            m_series->begin(),
            m_series->end(),
            [firstRelease] (ReleaseSeriesModel* model) {
                return model->releaseIds()->contains(firstRelease);
            }
        );
        if (anotherSeriesItem != m_series->end()) m_series->removeOne(*anotherSeriesItem);
    }

    // recalculate and save series
    series->recalculateCountReleases();

    if (series->countReleases() >= 2) m_series->append(series);
}

void ReleaseLinkedSeries::saveSeries()
{
    QJsonArray seriesArray;

    foreach (auto series, *m_series) {
        QJsonObject jsonObject;
        series->writeToJson(jsonObject);
        seriesArray.append(jsonObject);
    }

    QJsonDocument document(seriesArray);

    QFile file(getSeriesCachePath());
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qInfo() << "Can't write series file!";
        return;
    }
    file.write(document.toJson());
    file.close();
}
