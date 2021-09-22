#ifndef NOTIFICATIONMODEL_H
#define NOTIFICATIONMODEL_H

#include <QDateTime>
#include <QUuid>

class NotificationModel
{
private:
    QDateTime m_timeStamp;
    QString m_message;
    int m_type;
    QUuid m_id;

public:
    NotificationModel();

    QDateTime timeStamp() const noexcept { return m_timeStamp; }
    QString message() const noexcept { return m_message; }
    int type() const noexcept { return m_type; }
    QString id() const noexcept { return m_id.toString(); }

    void setMessage(const QString& message) noexcept;
    void setType(const int& type) noexcept;
};

#endif // NOTIFICATIONMODEL_H
