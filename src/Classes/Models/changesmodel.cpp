#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "changesmodel.h"

ChangesModel::ChangesModel() :
    m_NewReleases(new QList<int>()),
    m_NewOnlineSeries(new QList<int>()),
    m_NewTorrents(new QList<int>()),
    m_NewTorrentSeries(new QList<int>())
{

}

QList<int>* ChangesModel::newReleases()
{
    return m_NewReleases;
}

QList<int>* ChangesModel::newTorrentSeries()
{
    return m_NewTorrentSeries;
}

QList<int>* ChangesModel::newOnlineSeries()
{
    return m_NewOnlineSeries;
}

QList<int>* ChangesModel::newTorrents()
{
    return m_NewTorrents;
}

void ChangesModel::fromJson(QString json)
{
    if (json.isEmpty()) return;

    auto document = QJsonDocument::fromJson(json.toUtf8());
    auto jsonChanges = document.object();

    auto newReleases = jsonChanges.value("newReleases").toArray();
    foreach (auto newRelease, newReleases) m_NewReleases->append(newRelease.toInt());

    auto newTorrents = jsonChanges.value("newTorrents").toArray();
    foreach (auto newTorrent, newTorrents) m_NewTorrents->append(newTorrent.toInt());

    auto newOnlineSeries = jsonChanges.value("newOnlineSeries").toArray();
    foreach (auto newOnlineSeria, newOnlineSeries) m_NewOnlineSeries->append(newOnlineSeria.toInt());

    auto newTorrentSeries = jsonChanges.value("newTorrentSeries").toArray();
    foreach (auto newTorrentSeria, newTorrentSeries) m_NewTorrentSeries->append(newTorrentSeria.toInt());
}

QString ChangesModel::toJson()
{
    QJsonObject object;

    QJsonArray newReleases;
    foreach (auto newRelease, *m_NewReleases) {
        QJsonValue newReleaseValue(newRelease);
        newReleases.append(newReleaseValue);
    }
    object["newReleases"] = newReleases;

    QJsonArray newTorrentSeries;
    foreach (auto newTorrentSerie, *m_NewTorrentSeries) {
        QJsonValue newTorrentSerieValue(newTorrentSerie);
        newTorrentSeries.append(newTorrentSerieValue);
    }
    object["newTorrentSeries"] = newTorrentSeries;

    QJsonArray newOnlineSeries;
    foreach (auto newOnlineSerie, *m_NewOnlineSeries) {
        QJsonValue newOnlineSerieValue(newOnlineSerie);
        newOnlineSeries.append(newOnlineSerieValue);
    }
    object["newOnlineSeries"] = newOnlineSeries;

    QJsonArray newTorrents;
    foreach (auto newTorrent, *m_NewTorrents) {
        QJsonValue newTorrentItem(newTorrent);
        newTorrents.append(newTorrentItem);
    }
    object["newTorrents"] = newTorrents;

    QJsonDocument saveDoc(object);
    return saveDoc.toJson();
}
