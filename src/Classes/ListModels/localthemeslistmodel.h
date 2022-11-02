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

#ifndef LOCALTHEMESLISTMODEL_H
#define LOCALTHEMESLISTMODEL_H

#include <QAbstractListModel>

class LocalThemesListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool listIsEmpty READ listIsEmpty NOTIFY listIsEmptyChanged)

private:
    enum ExternalThemesRoles {
        IndexRole = Qt::UserRole + 1,
        TitleRole
    };
    QList<QString>* m_items { nullptr };

public:
    explicit LocalThemesListModel(QObject *parent = nullptr);

    void setItems(QList<QString>* items) noexcept;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    bool listIsEmpty() const noexcept { return m_items->isEmpty(); }

    void refresh();

signals:
    void listIsEmptyChanged();

};

#endif // LOCALTHEMESLISTMODEL_H
