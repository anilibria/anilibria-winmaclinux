#include "notificationviewmodel.h"

NotificationViewModel::NotificationViewModel(QObject *parent) : QAbstractListModel(parent),
    m_notifications(new QList<NotificationModel*>())
{

}

int NotificationViewModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_notifications->size();
}

QVariant NotificationViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto notification = m_notifications->at(index.row());

    switch (role) {
        case TimeStampRole: {
            return QVariant(notification->timeStamp());
        }
        case TypeRole: {
            return QVariant(getType(notification->type()));
        }
        case MessageRole: {
            return QVariant(notification->message());
        }
        case IdRole: {
            return QVariant(notification->id());
        }
    }

    return QVariant();
}

QHash<int, QByteArray> NotificationViewModel::roleNames() const
{
    return {
        {
            TimeStampRole,
            "timeStamp"
        },
        {
            TypeRole,
            "type"
        },
        {
            MessageRole,
            "message"
        },
        {
            IdRole,
            "id"
        }
    };
}

void NotificationViewModel::sendInfoNotification(const QString &message)
{
    addNewNotification(InfoType, message);
}

void NotificationViewModel::sendErrorNotification(const QString &message)
{
    addNewNotification(ErrorType, message);
}

void NotificationViewModel::clearAllNotifications()
{
    beginResetModel();

    m_notifications->clear();

    endResetModel();

    emit countNotificationsChanged();
}

void NotificationViewModel::clearNotification(const QString &id)
{
    auto result = std::find_if(
        m_notifications->begin(),
        m_notifications->end(),
        [id] (NotificationModel* model) {
            return model->id() == id;
        }
    );
    if (result == m_notifications->end()) return;

    beginResetModel();

    m_notifications->removeOne(*result);

    endResetModel();

    emit countNotificationsChanged();
}

QString NotificationViewModel::getType(const int &type) const noexcept
{
    switch (type) {
        case InfoType:
            return "info";
        case ErrorType:
            return "error";
        case NewsType:
            return "news";
        default:
            return "";
    }
}

void NotificationViewModel::addNewNotification(const int &type, const QString &message)
{
    beginResetModel();

    auto model = new NotificationModel();
    model->setType(type);
    model->setMessage(message);
    m_notifications->insert(0, model);

    endResetModel();

    emit countNotificationsChanged();
}

