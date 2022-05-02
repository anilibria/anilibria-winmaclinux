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

#include "commoncomboboxlistmodel.h"

CommonComboBoxListModel::CommonComboBoxListModel(QObject *parent) : QAbstractListModel(parent)
{

}

void CommonComboBoxListModel::addItem(int id, QString title) noexcept
{
    m_items->append(std::make_tuple(id, title));
    if (m_items->count() == 1) setSelectedItem(id);
}

int CommonComboBoxListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_items->size();
}

QVariant CommonComboBoxListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto item = m_items->at(index.row());

    switch (role) {
        case IdRole: {
            return QVariant(std::get<0>(item));
        }
        case TitleRole: {
            return QVariant(std::get<1>(item));
        }
        default: return QVariant();
    }
}

QHash<int, QByteArray> CommonComboBoxListModel::roleNames() const
{
    return {
        {
            IdRole,
            "id"
        },
        {
            TitleRole,
            "title"
        }
    };
}

void CommonComboBoxListModel::setSelectedItem(int selectedItem) noexcept
{
    if (m_selectedItem == selectedItem) return;

    m_selectedItem = selectedItem;
    emit selectedItemChanged();
}
