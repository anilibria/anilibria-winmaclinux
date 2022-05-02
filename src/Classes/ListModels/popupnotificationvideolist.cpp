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

#include "popupnotificationvideolist.h"
#include <QListIterator>
#include <QDebug>

PopupNotificationVideoList::PopupNotificationVideoList(QObject *parent) : QAbstractListModel(parent),
    m_popupNotifications(new QList<NotificationModel*>()),
    m_timer(new QTimer(parent)),
    m_showNotifications(false)
{
    m_timer->setInterval(1000);
    connect(m_timer, &QTimer::timeout, this, &PopupNotificationVideoList::timeout);
}

int PopupNotificationVideoList::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_popupNotifications->size();
}

QVariant PopupNotificationVideoList::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto notification = m_popupNotifications->at(index.row());

    switch (role) {
        case IdRole: {
            return QVariant(notification->id());
        }
        case MessageRole: {
            return QVariant(notification->message());
        }
    }

    return QVariant();
}

QHash<int, QByteArray> PopupNotificationVideoList::roleNames() const
{
    return {
        {
            IdRole,
            "id"
        },
        {
            MessageRole,
            "message"
        }
    };
}

void PopupNotificationVideoList::addNewNotification(const int &type, const QString &message)
{
    beginResetModel();

    NotificationModel* model = new NotificationModel();
    model->setType(type);
    model->setMessage(message);

    m_popupNotifications->append(model);

    if (!m_timer->isActive()) m_timer->start();

    endResetModel();

    setShowNotifications(true);
}

void PopupNotificationVideoList::setShowNotifications(const bool &showNotifications)
{
    if (m_showNotifications == showNotifications) return;

    m_showNotifications = showNotifications;
    emit showNotificationsChanged();
}

void PopupNotificationVideoList::timeout()
{
    auto result = std::find_if(
        m_popupNotifications->begin(),
        m_popupNotifications->end(),
        [](NotificationModel* model) {
            auto now = QDateTime::currentDateTime();
            auto timeStamp = model->timeStamp();
            return timeStamp.secsTo(now) > 3;
        }
    );
    if (result == m_popupNotifications->end()) return;

    beginResetModel();

    m_popupNotifications->removeOne(*result);

    endResetModel();

    if (m_popupNotifications->empty()) {
        m_timer->stop();
        setShowNotifications(false);
    }
}
