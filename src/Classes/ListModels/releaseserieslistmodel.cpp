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

#include "releaseserieslistmodel.h"

ReleaseSeriesListModel::ReleaseSeriesListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

void ReleaseSeriesListModel::refresh() noexcept
{
    if (m_linkedSeries == nullptr) return;

    beginResetModel();

    m_releases->clear();

    if (m_releaseId > 0) {
        auto releases = m_releases.get();

        m_linkedSeries->fillReleaseSeries(releases, m_releaseId);
    }

    endResetModel();

    emit isEmptyChanged();
}

int ReleaseSeriesListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_releases->size();
}

QVariant ReleaseSeriesListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto release = m_releases->at(index.row());

    switch (role) {
        case IndexRole: {
            return QVariant(release->id());
        }
        case TitleRole: {
            return QVariant(release->title());
        }
        case PosterRole: {
            return QVariant(release->poster());
        }
        case IsSelected: {
            return QVariant(release->id() == m_releaseId);
        }
        case ReleaseNumber: {
            return QVariant(index.row() + 1);
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ReleaseSeriesListModel::roleNames() const
{
    return {
        {
            IndexRole,
            "id"
        },
        {
            TitleRole,
            "title"
        },
        {
            PosterRole,
            "poster"
        },
        {
            IsSelected,
            "isSelected",
        },
        {
            ReleaseNumber,
            "releaseNumber"
        }
    };
}

void ReleaseSeriesListModel::setLinkedSeries(const ReleaseLinkedSeries *linkedSeries) noexcept
{
    if (m_linkedSeries == linkedSeries) return;

    m_linkedSeries = const_cast<ReleaseLinkedSeries *>(linkedSeries);
    emit linkedSeriesChanged();
}

void ReleaseSeriesListModel::setReleaseId(int releaseId) noexcept
{
    if (m_releaseId == releaseId) return;

    m_releaseId = releaseId;
    emit releaseIdChanged();

    refresh();
}
