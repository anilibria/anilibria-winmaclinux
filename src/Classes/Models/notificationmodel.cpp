#include "notificationmodel.h"

NotificationModel::NotificationModel() : m_timeStamp(QDateTime::currentDateTime()), m_message(""), m_id(QUuid::createUuid())
{

}

void NotificationModel::setMessage(const QString &message) noexcept
{
    m_message = message;
}

void NotificationModel::setType(const int &type) noexcept
{
    m_type = type;
}
