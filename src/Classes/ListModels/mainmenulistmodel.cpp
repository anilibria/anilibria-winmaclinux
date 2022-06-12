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

#include "mainmenulistmodel.h"
#include <QDebug>

MainMenuListModel::MainMenuListModel(QObject *parent) : QAbstractListModel(parent)
{
    m_items->append(new MainMenuItemModel(0, "Каталог релизов", "release", "catalogmenu.svg"));
    m_items->append(new MainMenuItemModel(1, "Видеоплеер", "videoplayer", "videoplayermenu.svg"));
    m_items->append(new MainMenuItemModel(2, "Моя Анилибрия", "myanilibria", "house.svg"));
    m_items->append(new MainMenuItemModel(3, "Кинозал", "cinemahall", "cinemahallmenu.svg"));
    m_items->append(new MainMenuItemModel(4, "Связанные релизы", "releaseseries", "seriesmenu.svg"));
    m_items->append(new MainMenuItemModel(5, "Менеджер загрузок", "download", "downloadcircle.svg"));
    m_items->append(new MainMenuItemModel(6, "Youtube", "youtube", "youtube.svg"));
    m_items->append(new MainMenuItemModel(7, "Обслуживание", "maintenance", "maintenance.svg"));
    m_items->append(new MainMenuItemModel(8, "Поддержать проект", "", "donate.svg"));
}

int MainMenuListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_items->size();
}

QVariant MainMenuListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto item = m_items->at(index.row());

    switch (role) {
        case IndexRole: {
            return QVariant(item->index());
        }
        case TitleRole: {
            return QVariant(item->title());
        }
        case PageNameRole: {
            return QVariant(item->pageName());
        }
        case IconRole: {
            return QVariant(item->icon());
        }
    }

    return QVariant();
}

QHash<int, QByteArray> MainMenuListModel::roleNames() const
{
    return {
        {
            IndexRole,
            "pageIndex"
        },
        {
            TitleRole,
            "title"
        },
        {
            PageNameRole,
            "pageName"
        },
        {
            IconRole,
            "icon"
        }
    };
}

void MainMenuListModel::setAuthorizeItemVisible(bool visible)
{
    beginResetModel();

    if (visible) {
        auto iterator = std::find_if(
            m_items->begin(),
            m_items->end(),
            [] (MainMenuItemModel* mainMenuItem) {
                return mainMenuItem->index() == 9;
            }
        );

        if (iterator != m_items->end()) {
            m_items->removeOne(*iterator);
        }
    } else {
        m_items->append(new MainMenuItemModel(9, "Войти", "authorization", "user.svg"));
    }

    endResetModel();
}

void MainMenuListModel::selectItem(int index)
{
    m_selectedItem = index;

    auto iterator = std::find_if(
        m_items->begin(),
        m_items->end(),
        [index](MainMenuItemModel* mainMenuItem) {
            return mainMenuItem->index() == index;
        }
    );
    if (iterator == m_items->end()) return;

    auto menuItem = *iterator;

    emit selectedItem(index, menuItem->pageName());
}
