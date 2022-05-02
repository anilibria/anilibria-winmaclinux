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

#ifndef COMMONCOMBOBOXLISTMODEL_H
#define COMMONCOMBOBOXLISTMODEL_H

#include <QObject>
#include <QList>
#include <QAbstractListModel>

class CommonComboBoxListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int selectedItem READ selectedItem WRITE setSelectedItem NOTIFY selectedItemChanged)

private:
    QList<std::tuple<int, QString>>* m_items { new QList<std::tuple<int, QString>>() };
    int m_selectedItem { -1 };

    enum CommonItemRoles {
        IdRole = Qt::UserRole + 1,
        TitleRole
    };

public:
    explicit CommonComboBoxListModel(QObject *parent = nullptr);

    void addItem(int id, QString title) noexcept;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    int selectedItem() const noexcept { return m_selectedItem; }
    void setSelectedItem(int selectedItem) noexcept;

signals:
    void selectedItemChanged();

};

#endif // COMMONCOMBOBOXLISTMODEL_H
