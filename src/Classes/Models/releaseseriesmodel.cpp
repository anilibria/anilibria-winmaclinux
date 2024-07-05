#include "releaseseriesmodel.h"
#include <QJsonArray>

ReleaseSeriesModel::ReleaseSeriesModel()
{

}

ReleaseSeriesModel::~ReleaseSeriesModel()
{
    m_releaseIds->clear();
    m_posters->clear();
    m_titles->clear();
}

QString ReleaseSeriesModel::titleAsString()
{
    if (m_titlesAsString.isEmpty() && !m_titles->isEmpty()) {
        m_titlesAsString = m_titles->join(", ");
    }

    return m_titlesAsString;
}

QString ReleaseSeriesModel::genresAsString()
{
    if (m_genresAsString.isEmpty() && !m_genres->isEmpty()) {
        m_genresAsString = m_genres->join(", ");
    }

    return m_genresAsString;
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
    m_titlesAsString = m_titles->join(", ");

    if (jsonObject.contains("title")) m_title = jsonObject.value("title").toString();

    if (jsonObject.contains("genres")) {
        auto genres = jsonObject.value("genres").toArray();
        m_genres->clear();
        foreach (auto genre, genres) m_genres->append(genre.toString());

        std::sort(
            m_genres->begin(),
            m_genres->end(),
            [](const QString & left, const QString & right) {
                return left < right;
            }
        );
        m_genresAsString = m_genres->join(", ");
    }
}
