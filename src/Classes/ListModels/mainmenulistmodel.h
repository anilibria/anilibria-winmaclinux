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

#ifndef MAINMENULISTMODEL_H
#define MAINMENULISTMODEL_H

#include <QObject>
#include <QList>
#include <QModelIndex>
#include "../Models/mainmenuitemmodel.h"

class MainMenuListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit MainMenuListModel(QObject *parent = nullptr);

    QList<MainMenuItemModel*>* m_items { new QList<MainMenuItemModel*>() };

    int m_authorizationOrder { 0 };

    int m_selectedItem { 0 };

    enum MainMenuRoles {
        IndexRole = Qt::UserRole + 1,
        TitleRole,
        PageNameRole,
        IconRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int,QByteArray> roleNames() const override;

    void setAuthorizeItemVisible(bool visible);

    Q_INVOKABLE void selectItem(int index);

signals:    
    void selectedItem(QString pageName);

};

#endif // MAINMENULISTMODEL_H
