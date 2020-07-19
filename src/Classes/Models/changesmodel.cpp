/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2020 Roman Vladimirov

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

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "changesmodel.h"

ChangesModel::ChangesModel() noexcept :
    m_NewReleases(new QList<int>()),
    m_NewOnlineSeries(new QList<int>()),
    m_NewTorrents(new QList<int>()),
    m_NewTorrentSeries(new QList<int>())
{

}

void ChangesModel::fromJson(const QString& json)
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

QString ChangesModel::toJson() noexcept
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
