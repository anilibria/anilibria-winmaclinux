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

#ifndef RELEASESIMPLELISTMODEL_H
#define RELEASESIMPLELISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include "../Models/fullreleasemodel.h"
#include "../ViewModels/releasesviewmodel.h"

class ReleaseSimpleListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ReleasesViewModel* releases READ releases WRITE setReleases NOTIFY releasesChanged)
    Q_PROPERTY(QString filterMode READ filterMode WRITE setFilterMode NOTIFY filterModeChanged)
    Q_PROPERTY(QString filterNewInFavorites READ filterNewInFavorites NOTIFY filterNewInFavoritesChanged)
    Q_PROPERTY(QString filterNewFromStart READ filterNewFromStart NOTIFY filterNewFromStartChanged)
    Q_PROPERTY(bool hasItems READ hasItems NOTIFY hasItemsChanged)
    Q_PROPERTY(int countItems READ countItems NOTIFY countItemsChanged)

private:
    QScopedPointer<QList<FullReleaseModel*>> m_releases { new QList<FullReleaseModel*>() };
    ReleasesViewModel* m_releasesViewModel { nullptr };
    QString m_filterMode { "" };

public:

    explicit ReleaseSimpleListModel(QObject *parent = nullptr);

    enum ReleaseSimpleRoles {
        IndexRole = Qt::UserRole + 1,
        TitleRole,
        PosterRole,
        DescriptionRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int,QByteArray> roleNames() const override;

    QString filterNewInFavorites() const noexcept { return "newinfavorites"; };

    QString filterNewFromStart() const noexcept { return "newfromstart"; }

    ReleasesViewModel* releases() const noexcept { return m_releasesViewModel; }
    void setReleases(const ReleasesViewModel* viewModel) noexcept;

    QString filterMode() const noexcept { return m_filterMode; }
    void setFilterMode(const QString& filterMode) noexcept;

    bool hasItems() const noexcept { return !m_releases->isEmpty(); }

    int countItems() const noexcept { return m_releases->count(); }

private:
    void refresh();

signals:
    void releasesChanged();
    void filterModeChanged();
    void filterNewInFavoritesChanged();
    void hasItemsChanged();
    void previousApplicationStartChanged();
    void filterNewFromStartChanged();
    void countItemsChanged();

};

#endif // RELEASESIMPLELISTMODEL_H
