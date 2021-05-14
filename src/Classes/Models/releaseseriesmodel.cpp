#include "releaseseriesmodel.h"
#include <QJsonArray>

ReleaseSeriesModel::ReleaseSeriesModel() : m_releaseIds(new QList<QVariant>()), m_posters(new QList<QVariant>()), m_countReleases(0), m_firstName(""), m_secondName(""), m_thirdName("")
{

}

void ReleaseSeriesModel::recalculateCountReleases()
{
    m_countReleases = m_releaseIds->length();
}

void ReleaseSeriesModel::setFirstName(const QString &firstname)
{
    m_firstName = firstname;
}

void ReleaseSeriesModel::setSecondName(const QString &secondname)
{
    m_secondName = secondname;
}

void ReleaseSeriesModel::setThirdName(const QString &thirdname)
{
    m_thirdName = thirdname;
}

void ReleaseSeriesModel::setFirstPoster(const QString &firstposter)
{
    m_firstPoster = firstposter;
}

void ReleaseSeriesModel::setSecondPoster(const QString &secondposter)
{
    m_secondPoster = secondposter;
}

void ReleaseSeriesModel::setThirdPoster(const QString &thirdposter)
{
    m_thirdPoster = thirdposter;
}

bool ReleaseSeriesModel::appendReleaseId(const int id)
{
    if (m_releaseIds->contains(id)) return false;

    m_releaseIds->append(QVariant(id));
    return true;
}

void ReleaseSeriesModel::appendPoster(const QString &poster)
{
    m_posters->append(QVariant(poster));
}

void ReleaseSeriesModel::readFromJson(const QJsonObject &jsonObject) noexcept
{
    m_countReleases = jsonObject.value("countReleases").toInt();
    m_firstName = jsonObject.value("firstName").toString();
    m_secondName = jsonObject.value("secondName").toString();
    m_thirdName = jsonObject.value("thirdName").toString();
    m_firstPoster = jsonObject.value("firstPoster").toString();
    m_secondPoster = jsonObject.value("secondPoster").toString();
    m_thirdPoster = jsonObject.value("thirdPoster").toString();
    auto releases = jsonObject.value("releasesIds").toArray();
    m_releaseIds->clear();
    foreach (auto release, releases) m_releaseIds->append(QVariant(release.toInt()));

    auto posters = jsonObject.value("posters").toArray();
    m_posters->clear();
    foreach (auto poster, posters) m_posters->append(QVariant(poster));
}

void ReleaseSeriesModel::writeToJson(QJsonObject &json) const noexcept
{
    json["countReleases"] = m_releaseIds->count();
    json["firstName"] = m_firstName;
    json["secondName"] = m_secondName;
    json["thirdName"] = m_thirdName;
    json["firstPoster"] = m_firstPoster;
    json["secondPoster"] = m_secondPoster;
    json["thirdPoster"] = m_thirdPoster;

    QJsonArray releases;
    foreach (auto release, *m_releaseIds) releases.append(release.toInt());
    json["releasesIds"] = releases;

    QJsonArray posters;
    foreach (auto poster, *m_posters) posters.append(poster.toString());
    json["posters"] = posters;
}
