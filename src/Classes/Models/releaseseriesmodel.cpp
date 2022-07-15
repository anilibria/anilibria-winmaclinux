#include "releaseseriesmodel.h"
#include <QJsonArray>

ReleaseSeriesModel::ReleaseSeriesModel() :
    m_releaseIds(new QList<QVariant>()),
    m_posters(new QList<QVariant>()),
    m_countReleases(0)
{

}

ReleaseSeriesModel::~ReleaseSeriesModel()
{
    m_releaseIds->clear();
    m_posters->clear();
    m_titles->clear();
}

void ReleaseSeriesModel::recalculateCountReleases()
{
    m_countReleases = m_releaseIds->length();
}

bool ReleaseSeriesModel::appendReleaseId(const int id)
{
    if (m_releaseIds->contains(id)) return false;

    m_releaseIds->append(id);
    return true;
}

void ReleaseSeriesModel::appendPoster(const QString &poster)
{
    m_posters->append(QVariant(poster));
}

void ReleaseSeriesModel::appendTitle(const QString &title)
{
    m_titles->append(title);
}

void ReleaseSeriesModel::appendGenre(const QString &genre)
{
    if (m_genres->contains(genre)) return;

    m_genres->append(genre);
}

void ReleaseSeriesModel::readFromJson(const QJsonObject &jsonObject) noexcept
{
    m_countReleases = jsonObject.value("countReleases").toInt();
    auto releases = jsonObject.value("releasesIds").toArray();
    m_releaseIds->clear();
    foreach (auto release, releases) m_releaseIds->append(release.toInt());

    auto posters = jsonObject.value("posters").toArray();
    m_posters->clear();
    foreach (auto poster, posters) m_posters->append(QVariant(poster.toString()));

    auto titles = jsonObject.value("titles").toArray();
    m_titles->clear();
    foreach (auto title, titles) m_titles->append(title.toString());

    if (jsonObject.contains("genres")) {
        auto genres = jsonObject.value("genres").toArray();
        m_genres->clear();
        foreach (auto genre, genres) m_genres->append(genre.toString());
    }
}

void ReleaseSeriesModel::writeToJson(QJsonObject &json) const noexcept
{
    json["countReleases"] = m_releaseIds->count();

    QJsonArray releases;
    foreach (auto release, *m_releaseIds) releases.append(release.toInt());
    json["releasesIds"] = releases;

    QJsonArray posters;
    foreach (auto poster, *m_posters) posters.append(poster.toString());
    json["posters"] = posters;

    QJsonArray titles;
    foreach (auto title, *m_titles) titles.append(title);
    json["titles"] = titles;

    QJsonArray genres;
    foreach (auto genre, *m_genres) genres.append(genre);
    json["genres"] = genres;
}
