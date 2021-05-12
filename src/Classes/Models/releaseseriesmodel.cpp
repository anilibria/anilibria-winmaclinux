#include "releaseseriesmodel.h"
#include <QJsonArray>

ReleaseSeriesModel::ReleaseSeriesModel() : m_releaseIds(new QList<QVariant>()), m_countReleases(0), m_firstName(""), m_secondName(""), m_thirdName("")
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

bool ReleaseSeriesModel::appendReleaseId(const int id)
{
    if (m_releaseIds->contains(id)) return false;

    m_releaseIds->append(QVariant(id));
    return true;
}

void ReleaseSeriesModel::readFromJson(const QJsonObject &jsonObject) noexcept
{
    m_countReleases = jsonObject.value("countReleases").toInt();
    m_firstName = jsonObject.value("firstName").toString();
    m_secondName = jsonObject.value("secondName").toString();
    m_thirdName = jsonObject.value("thirdName").toString();
    auto releases = jsonObject.value("releassIds").toArray();
    m_releaseIds->clear();
    foreach (auto release, releases) {
        m_releaseIds->append(QVariant(release.toInt()));
    }

}

void ReleaseSeriesModel::writeToJson(QJsonObject &json) const noexcept
{
    json["countReleases"] = m_releaseIds->count();
    json["firstName"] = m_firstName;
    json["secondName"] = m_secondName;
    json["thirdName"] = m_thirdName;

    QJsonArray releases;
    foreach (auto release, *m_releaseIds) releases.append(release.toInt());
    json["releassIds"] = releases;
}
