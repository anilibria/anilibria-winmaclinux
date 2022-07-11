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

#ifndef RELEASELINKEDSERIES_H
#define RELEASELINKEDSERIES_H


#include <QObject>
#include <QAbstractListModel>
#include <QVariant>
#include <QScopedPointer>
#include <QFutureWatcher>
#include "../Models/fullreleasemodel.h"
#include "../Models/releaseseriesmodel.h"

class ReleaseLinkedSeries : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString nameFilter READ nameFilter WRITE setNameFilter NOTIFY nameFilterChanged)
private:
    QString m_nameFilter;
    QScopedPointer<QVector<ReleaseSeriesModel*>> m_series;
    QScopedPointer<QVector<ReleaseSeriesModel*>> m_filteredSeries;
    bool m_filtering = false;
    QSharedPointer<QList<FullReleaseModel *>> m_releases;
    QScopedPointer<QFutureWatcher<bool>> m_cacheUpdateWatcher { new QFutureWatcher<bool>(this) };

    enum ItemRoles {
        CountReleasesRole = Qt::UserRole + 1,
        FirstThreeNamesRole,
        ReleaseIds,
        Posters,
        FirstPosterRole,
        SecondPosterRole,
        ThirdPosterRole,
        OtherReleasesRole,
        GenresRole,
    };

public:
    explicit ReleaseLinkedSeries(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    void setup(QSharedPointer<QList<FullReleaseModel *>> releases);

    QString nameFilter() const { return m_nameFilter; }
    void setNameFilter(const QString& nameFilter) noexcept;

    QSharedPointer<QList<int>> getAllLinkedReleases() const noexcept;
    int getSortedOrder(int id) const noexcept;

    void fillReleaseSeries(QList<FullReleaseModel*>* list, const int id) noexcept;

    Q_INVOKABLE int getNextLinkedRelease(const int currentRelease);
    Q_INVOKABLE void refreshSeries();
    Q_INVOKABLE bool isReleaseInSeries(int id);
    Q_INVOKABLE void filterSeries();
    Q_INVOKABLE void clearFilters();    

private:
    QString getSeriesCachePath() const noexcept;
    QString getReleasesCachePath() const noexcept;
    void loadSeries();
    void createCacheFileIfNotExists() const noexcept;
    void processReleasesFromDescription(const QString& description, const QMap<QString, FullReleaseModel*>& releases, int currentRelease, const QString currentReleaseTitle, const QString& poster, const QString& genres) noexcept;
    void saveSeries();

private slots:
    void cacheUpdated();

signals:
    void nameFilterChanged();

};

#endif // RELEASELINKEDSERIES_H
