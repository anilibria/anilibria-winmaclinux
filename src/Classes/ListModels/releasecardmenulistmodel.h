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

#ifndef RELEASECARDMENULISTMODEL_H
#define RELEASECARDMENULISTMODEL_H

#include <QAbstractListModel>

class ReleaseCardMenuListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString selectedTitle READ selectedTitle NOTIFY selectedTitleChanged)
    Q_PROPERTY(bool isReleaseSeries READ isReleaseSeries NOTIFY isReleaseSeriesChanged)
    Q_PROPERTY(bool isOnlineVideos READ isOnlineVideos NOTIFY isOnlineVideosChanged)

private:
    QStringList m_items { QStringList() };
    int m_selectedItem { 0 };
    enum ReleaseCardMenuRoles {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        IsSelectedRole
    };

public:
    explicit ReleaseCardMenuListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    QString selectedTitle() const noexcept { return m_items.value(m_selectedItem); }
    bool isReleaseSeries() const noexcept { return m_selectedItem == 0; }
    bool isOnlineVideos() const noexcept { return m_selectedItem == 1; }

    Q_INVOKABLE void select(int id) noexcept;

signals:
    void selectedTitleChanged();
    void isReleaseSeriesChanged();
    void isOnlineVideosChanged();

};

#endif // RELEASECARDMENULISTMODEL_H
