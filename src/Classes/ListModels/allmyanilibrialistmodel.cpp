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

#include "allmyanilibrialistmodel.h"

AllMyAnilibriaListModel::AllMyAnilibriaListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

void AllMyAnilibriaListModel::setup(QSharedPointer<QSet<QString> > fullSections, QSharedPointer<QMap<QString, QString>> sectionTitles, QSharedPointer<QSet<QString>> selectedSections)
{
    m_fullSections = fullSections;
    m_sectionTitles = sectionTitles;
    m_selectedSections = selectedSections;
}

int AllMyAnilibriaListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_fullSections->size();
}

QVariant AllMyAnilibriaListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto section = m_fullSections->values().value(index.row());
    auto title = m_sectionTitles->value(section);
    auto isSelected = m_selectedSections->contains(section);
    auto indexSection = m_selectedSections->values().indexOf(section);

    switch (role) {
        case SectionIdRole: {
            return QVariant(section);
        }
        case SectionTitleRole: {
            return QVariant(title);
        }
        case SectionSelectedRole: {
            return QVariant(isSelected);
        }
        case SectionIndexRole: {
            return QVariant(indexSection);
        }
    }

    return QVariant();
}

QHash<int, QByteArray> AllMyAnilibriaListModel::roleNames() const
{
    return {
        {
            SectionIdRole,
            "sectionId"
        },
        {
            SectionTitleRole,
            "sectionTitle"
        },
        {
            SectionSelectedRole,
            "sectionSelected"
        },
        {
            SectionIndexRole,
            "sectionIndex"
        }
    };

}

void AllMyAnilibriaListModel::refreshData()
{
    beginResetModel();
    endResetModel();
}
