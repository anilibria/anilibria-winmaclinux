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

#include <QDateTime>
#include "externalthemeslistmodel.h"

ExternalThemesListModel::ExternalThemesListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

void ExternalThemesListModel::setItems(const QList<ThemeItemModel *> &items, QList<QString>* externals) noexcept
{
    beginResetModel();

    m_items = items;
    m_externals = externals;

    endResetModel();

    emit listIsEmptyChanged();
}

int ExternalThemesListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_items.size();
}

QVariant ExternalThemesListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto itemIndex = index.row();
    auto item = m_items.at(itemIndex);

    switch (role) {
        case IndexRole: {
            return QVariant(itemIndex);
        }
        case TitleRole: {
            return QVariant(item->name());
        }
        case ScreenshootRole: {
            return QVariant("data:image/png;base64," + item->screenshoot());
        }
        case AuthorRole: {
            return QVariant(item->author());
        }
        case VersionRole: {
            return QVariant(item->version());
        }
        case LastUpdateRole: {
            return QVariant(QDateTime::fromSecsSinceEpoch(item->lastUpdated()).toString("dd.MM.yyyy"));
        }
        case IsDownloadedRole: {
            qDebug() << item->source() << " " << m_externals->contains(item->source());
            return QVariant(m_externals != nullptr ? m_externals->contains(item->source()) : false);
        }
        case ExternalIdRole: {
            return QVariant(item->source());
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ExternalThemesListModel::roleNames() const
{
    return {
        {
            IndexRole,
            "themeIndex"
        },
        {
            TitleRole,
            "title"
        },
        {
            ScreenshootRole,
            "screenshoots"
        },
        {
            AuthorRole,
            "author"
        },
        {
            VersionRole,
            "version"
        },
        {
            LastUpdateRole,
            "lastUpdated"
        },
        {
            IsDownloadedRole,
            "isDownloaded"
        },
        {
            ExternalIdRole,
            "externalId"
        }
    };
}

ThemeItemModel *ExternalThemesListModel::getThemeByIndex(int index) const noexcept
{
    if (index >= m_items.count()) return nullptr;

    return m_items.value(index);
}

void ExternalThemesListModel::refresh()
{
    beginResetModel();

    endResetModel();

    emit listIsEmptyChanged();
}
