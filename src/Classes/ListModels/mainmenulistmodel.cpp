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
#include "../../globalconstants.h"

MainMenuListModel::MainMenuListModel(QObject *parent) : QAbstractListModel(parent)
{
    m_items->append(new MainMenuItemModel(0, "Каталог релизов", "release", iconMainMenuReleasesField));
    m_items->append(new MainMenuItemModel(1, "Видеоплеер", "videoplayer", iconMainMenuVideoplayerField));
    m_items->append(new MainMenuItemModel(2, "Моя Анилибрия", "myanilibria", iconMainMenuMyAnilibriaField));
    m_items->append(new MainMenuItemModel(3, "Кинозал", "cinemahall", iconMainMenuCinemahallField));
    m_items->append(new MainMenuItemModel(4, "Связанные релизы", "releaseseries", iconMainMenuReleasesSeriesField));
    m_items->append(new MainMenuItemModel(5, "TorrentStream", "torrentstream", iconMainMenuTorrentStreamField));
    m_items->append(new MainMenuItemModel(6, "Менеджер тем", "thememanager", iconMainMenuThemeManagerField));
    m_items->append(new MainMenuItemModel(7, "Youtube", "youtube", iconMainMenuYoutubeField));
    m_items->append(new MainMenuItemModel(8, "Обслуживание", "maintenance", iconMainMenuMaintenanceField));
    m_items->append(new MainMenuItemModel(9, "Приложения", "applications", iconMainMenuApplicationsField));
    m_items->append(new MainMenuItemModel(10, "Расширения", "extensions", iconMainMenuApplicationsField));
    m_items->append(new MainMenuItemModel(11, "Поддержать проект", "", iconMainMenuDonateField));

    m_authorizationOrder = m_items->count();
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

    auto authorizationOrder = m_authorizationOrder;

    if (visible) {
        auto iterator = std::find_if(
            m_items->begin(),
            m_items->end(),
            [authorizationOrder] (MainMenuItemModel* mainMenuItem) {
                return mainMenuItem->index() == authorizationOrder;
            }
        );

        if (iterator != m_items->end()) {
            m_items->removeOne(*iterator);
        }
    } else {
        m_items->append(new MainMenuItemModel(m_authorizationOrder, "Войти", "authorization", iconMainMenuAuthorizationField));
    }

    endResetModel();
}

QString MainMenuListModel::getIcon(const QString &id)
{
    auto item = std::find_if(
        m_items->cbegin(),
        m_items->cend(),
        [id](MainMenuItemModel* item) {
            return item->pageName() == id;
        }
    );

    return (*item)->icon();
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

    emit selectedItem(menuItem->pageName());
}
