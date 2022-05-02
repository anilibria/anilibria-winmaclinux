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

#include "commonmenulistmodel.h"

CommonMenuListModel::CommonMenuListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

void CommonMenuListModel::setup(const QStringList&& items) noexcept
{
    foreach (auto item, items) m_items->append(item);

    refresh();
}

void CommonMenuListModel::refresh() noexcept
{
    beginResetModel();
    m_filteredItems->clear();

    for (auto i = 0; i < m_items->count(); i++) {
        if (m_notVisibleItems->contains(i)) continue;

        m_filteredItems->append(i);
    }
    endResetModel();
}

int CommonMenuListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_filteredItems->size();
}

QVariant CommonMenuListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto itemIndex = m_filteredItems->at(index.row());
    auto item = m_items->at(itemIndex);

    switch (role) {
        case IndexRole: {
            return QVariant(itemIndex);
        }
        case TitleRole: {
            return QVariant(item);
        }
        case DisableRole: {
            return QVariant(m_disabledItems->contains(itemIndex));
        }
        default: return QVariant();
    }
}

QHash<int, QByteArray> CommonMenuListModel::roleNames() const
{
    return {
        {
            IndexRole,
            "itemIndex"
        },
        {
            TitleRole,
            "title"
        },
        {
            DisableRole,
            "itemDisabled"
        }
    };
}

void CommonMenuListModel::disableItem(const int index)
{
    if (m_disabledItems->contains(index)) return;

    m_disabledItems->insert(index);

    refresh();
}

void CommonMenuListModel::enableItem(const int index)
{
    if (!m_disabledItems->contains(index)) return;

    m_disabledItems->remove(index);

    refresh();
}

void CommonMenuListModel::hideItem(const int index)
{
    if (m_notVisibleItems->contains(index)) return;

    m_notVisibleItems->insert(index);

    refresh();
}

void CommonMenuListModel::showItem(const int index)
{
    if (!m_notVisibleItems->contains(index)) return;

    m_notVisibleItems->remove(index);

    refresh();
}

void CommonMenuListModel::selectItem(const int index)
{
    emit itemSelected(index);
}
