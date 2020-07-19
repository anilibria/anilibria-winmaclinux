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

#ifndef CHANGESMODEL_H
#define CHANGESMODEL_H

#include <QObject>
#include <QHash>

class ChangesModel
{

public:
    ChangesModel() noexcept;

private:
    QList<int>* m_NewReleases;
    QList<int>* m_NewOnlineSeries;
    QList<int>* m_NewTorrents;
    QList<int>* m_NewTorrentSeries;

public:
    QList<int>* newReleases() const { return m_NewReleases; }

    QList<int>* newOnlineSeries() const { return m_NewOnlineSeries; }

    QList<int>* newTorrents() const { return m_NewTorrents; }

    QList<int>* newTorrentSeries() const { return m_NewTorrentSeries; }

    void fromJson(const QString &json);

    QString toJson() noexcept;

};

#endif // CHANGESMODEL_H
