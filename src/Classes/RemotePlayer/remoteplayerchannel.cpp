#include <QObject>
#include <QStringList>
#include <QDebug>
#include <QTime>
#include "remoteplayerchannel.h"

RemotePlayerChannel::RemotePlayerChannel(QObject *parent) : QObject(parent)
{
    QTimer *t = new QTimer(this);
    connect(t, &QTimer::timeout, this, &RemotePlayerChannel::sendKeepAlive);
    t->start(10000);

    m_keepAliveCheckTimer = new QTimer(this);
    m_keepAliveCheckTimer->setSingleShot(true);
    m_keepAliveCheckTimer->setInterval(2000);
    connect(m_keepAliveCheckTimer, &QTimer::timeout, this, &RemotePlayerChannel::checkKeepAliveResponses);
}

bool RemotePlayerChannel::login(const QString &userName)
{
    //stop keepAliveCheck, when a new user logged in
    if (m_keepAliveCheckTimer->isActive()) {
        m_keepAliveCheckTimer->stop();
        m_stillAliveUsers.clear();
    }

    if (m_UserList.contains(userName)) {
        return false;
    }

    qDebug() << "User logged in:" << userName;
    m_UserList.append(userName);
    m_UserList.sort();
    emit userListChanged();
    emit userCountChanged();
    return true;
}

bool RemotePlayerChannel::logout(const QString &userName)
{
    if (!m_UserList.contains(userName)) {
        return false;
    } else {
        m_UserList.removeAt(m_UserList.indexOf(userName));
        emit userListChanged();
        emit userCountChanged();
        return true;
    }
}

bool RemotePlayerChannel::sendMessage(const QString &user, const QString &msg)
{
    if (m_UserList.contains(user)) {
        emit newMessage(QTime::currentTime().toString("HH:mm:ss"), user, msg);
        return true;
    } else {
        return false;
    }
}

void RemotePlayerChannel::keepAliveResponse(const QString &user)
{
    m_stillAliveUsers.append(user);
}

QStringList RemotePlayerChannel::userList() const
{
    return m_UserList;
}

void RemotePlayerChannel::sendKeepAlive()
{
    emit keepAlive();
    m_keepAliveCheckTimer->start();
}

void RemotePlayerChannel::checkKeepAliveResponses()
{
    qDebug() << "Keep Alive Check" << m_stillAliveUsers;
    m_UserList = m_stillAliveUsers;
    m_stillAliveUsers.clear();
    m_UserList.sort();
    emit userListChanged();
}
