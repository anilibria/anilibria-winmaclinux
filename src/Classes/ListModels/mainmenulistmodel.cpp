#include "mainmenulistmodel.h"
#include <QDebug>

MainMenuListModel::MainMenuListModel(QObject *parent) : QAbstractListModel(parent)
{
    m_items->append(new MainMenuItemModel(0, "Каталог релизов", "release", "catalogmenu.svg"));
    m_items->append(new MainMenuItemModel(1, "Видеоплеер", "videoplayer", "videoplayermenu.svg"));
    m_items->append(new MainMenuItemModel(2, "Моя Анилибрия", "myanilibria", "house.svg"));
    m_items->append(new MainMenuItemModel(3, "Youtube", "youtube", "youtube.svg"));
    m_items->append(new MainMenuItemModel(4, "Кинозал", "cinemahall", "cinemahallmenu.svg"));
    m_items->append(new MainMenuItemModel(5, "Связанные релизы", "releaseseries", "seriesmenu.svg"));
    m_items->append(new MainMenuItemModel(6, "Менеджер загрузок", "download", "downloadcircle.svg"));
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
