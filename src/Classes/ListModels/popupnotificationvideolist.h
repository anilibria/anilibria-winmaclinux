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

#ifndef POPUPNOTIFICATIONVIDEOLIST_H
#define POPUPNOTIFICATIONVIDEOLIST_H

#include <QObject>
#include <QAbstractListModel>
#include <QTimer>
#include "../Models/notificationmodel.h"

class PopupNotificationVideoList : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool showNotifications READ showNotifications NOTIFY showNotificationsChanged)

private:
    QList<NotificationModel*>* m_popupNotifications;
    QTimer* m_timer;
    bool m_showNotifications;

    enum NotificationRoles {
        IdRole = Qt::UserRole + 1,
        MessageRole
    };

public:
    explicit PopupNotificationVideoList(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;
    Q_INVOKABLE void addNewNotification(const int& type, const QString& message);

    bool showNotifications() const { return m_showNotifications; }
    void setShowNotifications(const bool& showNotifications);

signals:
    void showNotificationsChanged();

private slots:
    void timeout();

};

#endif // POPUPNOTIFICATIONVIDEOLIST_H
