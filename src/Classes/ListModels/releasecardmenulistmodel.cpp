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

#include "releasecardmenulistmodel.h"

ReleaseCardMenuListModel::ReleaseCardMenuListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_items.append("Связанные релизы");
    m_items.append("Серии");
}

int ReleaseCardMenuListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_items.size();
}

QVariant ReleaseCardMenuListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto currentIndex = index.row();
    auto menu = m_items.at(currentIndex);

    switch (role) {
        case IdRole: {
            return QVariant(currentIndex);
        }
        case TitleRole: {
            return QVariant(menu);
        }
        case IsSelectedRole: {
            return QVariant(m_selectedItem == currentIndex);
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ReleaseCardMenuListModel::roleNames() const
{
    return {
        {
            IdRole,
            "id"
        },
        {
            TitleRole,
            "title"
        },
        {
            IsSelectedRole,
            "isSelected"
        }
    };
}

void ReleaseCardMenuListModel::select(int id) noexcept
{
    beginResetModel();

    m_selectedItem = id;

    endResetModel();

    emit selectedTitleChanged();
    emit isReleaseSeriesChanged();
    emit isOnlineVideosChanged();
}
