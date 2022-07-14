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

#ifndef RELEASESERIESLISTMODEL_H
#define RELEASESERIESLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "../Models/fullreleasemodel.h"
#include "../Services/releaselinkedseries.h"

class ReleaseSeriesListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int releaseId READ releaseId WRITE setReleaseId NOTIFY releaseIdChanged)
    Q_PROPERTY(ReleaseLinkedSeries* linkedSeries READ linkedSeries WRITE setLinkedSeries NOTIFY linkedSeriesChanged)
    Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY isEmptyChanged)

private:
    enum ReleaseSimpleRoles {
        IndexRole = Qt::UserRole + 1,
        TitleRole,
        PosterRole,
        IsSelected,
        ReleaseNumber
    };
    QScopedPointer<QList<FullReleaseModel*>> m_releases { new QList<FullReleaseModel*>() };
    ReleaseLinkedSeries* m_linkedSeries { nullptr };
    int m_releaseId { -1 };

public:
    explicit ReleaseSeriesListModel(QObject *parent = nullptr);

    void refresh() noexcept;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    ReleaseLinkedSeries* linkedSeries() const noexcept { return m_linkedSeries; }
    void setLinkedSeries(const ReleaseLinkedSeries* linkedSeries) noexcept;

    int releaseId() const noexcept { return m_releaseId; }
    void setReleaseId(int releaseId) noexcept;

    bool isEmpty() const noexcept { return m_releases->isEmpty(); }

signals:
    void releaseIdChanged();
    void linkedSeriesChanged();
    void isEmptyChanged();

};

#endif // RELEASESERIESLISTMODEL_H
