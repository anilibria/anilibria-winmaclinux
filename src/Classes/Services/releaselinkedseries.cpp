#include <QFile>
#include <QDebug>
#include <QVector>
#include <QRegExp>
#include "releaselinkedseries.h"
#include "globalconstants.h"

ReleaseLinkedSeries::ReleaseLinkedSeries(QObject *parent) : QAbstractListModel(parent),
    m_countSeries(0),
    m_series(new QVector<ReleaseSeriesModel*>())
{
    createCacheFileIfNotExists();
    loadSeries();
}

int ReleaseLinkedSeries::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_series->size();
}

QVariant ReleaseLinkedSeries::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    switch (role) {
        case CountReleasesRole: {
            return QVariant(m_series->at(index.row())->countReleases());
        }
        case FirstNameRole: {
            return QVariant(m_series->at(index.row())->firstName());
        }
        case SecondNameRole: {
            return QVariant(m_series->at(index.row())->secondName());
        }
        case ThirdNameRole: {
            return QVariant(m_series->at(index.row())->thirdName());
        }
        case ReleaseIds: {
            return QVariant(*m_series->at(index.row())->releaseIds());
        }
        case Posters: {
            return QVariant(*m_series->at(index.row())->posters());
        }
        case FirstPosterRole: {
            return QVariant(m_series->at(index.row())->firstPoster());
        }
        case SecondPosterRole: {
            return QVariant(m_series->at(index.row())->secondPoster());
        }
        case ThirdPosterRole: {
            return QVariant(m_series->at(index.row())->thirdPoster());
        }
        case OtherReleasesRole: {
            auto count = m_series->at(index.row())->countReleases();

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

void ReleaseLinkedSeries::setCountSeries(const qint32 &countSeries) noexcept
{
    if (countSeries == m_countSeries) return;

    m_countSeries = countSeries;
    emit countSeriesChanged();
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
}

bool ReleaseLinkedSeries::isReleaseInSeries(int id)
{
    auto series = std::find_if(m_series->begin(), m_series->end(), [id](ReleaseSeriesModel* model) {
        return model->releaseIds()->contains(id);
    });

    return series != m_series->end();
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

void ReleaseLinkedSeries::setSeriaName(int index, QString name, ReleaseSeriesModel& model, const QString& poster)
{
    switch (index) {
        case 0:
            model.setFirstName(name);
            model.setFirstPoster(poster);
            break;
        case 1:
            model.setSecondName(name);
            model.setSecondPoster(poster);
            break;
        case 2:
            model.setThirdName(name);
            model.setThirdPoster(poster);
            break;
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

    int iterator = 0;
    foreach (auto part, parts) {
        auto partString = part.toString();

        if (linkRegexp.indexIn(partString, 0) > -1) {
            auto link = linkRegexp.cap(3);
            if (releases.contains(link)) {
                auto release = releases[link];
                if (series->appendReleaseId(release->id())) {
                    setSeriaName(iterator, release->title(), *series, release->poster());
                    series->appendPoster(release->poster());
                    iterator++;
                }
            }
        } else {
            if (series->appendReleaseId(currentRelease)) {
                setSeriaName(iterator, currentReleaseTitle, *series, poster);
                series->appendPoster(poster);
                iterator++;
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
