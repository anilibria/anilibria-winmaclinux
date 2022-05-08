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

#ifndef ALLMYANILIBRIALISTMODEL_H
#define ALLMYANILIBRIALISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QSharedPointer>
#include <QSet>
#include <QMap>

class AllMyAnilibriaListModel : public QAbstractListModel
{
    Q_OBJECT

private:
    QSharedPointer<QSet<QString>> m_fullSections { nullptr };
    QSharedPointer<QMap<QString, QString>> m_sectionTitles  { nullptr };
    QSharedPointer<QSet<QString>> m_selectedSections { nullptr };
    enum AllMyAnilibriaRoles {
        SectionIdRole = Qt::UserRole + 1,
        SectionTitleRole,
        SectionSelectedRole,
        SectionIndexRole
    };

public:
    explicit AllMyAnilibriaListModel(QObject *parent = nullptr);

    void setup(QSharedPointer<QSet<QString>> fullSections, QSharedPointer<QMap<QString, QString>> sectionTitles, QSharedPointer<QSet<QString>> selectedSections);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    void refreshData();

signals:

};

#endif // ALLMYANILIBRIALISTMODEL_H
