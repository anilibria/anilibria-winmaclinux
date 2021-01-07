#ifndef REMOTEPLAYERCHANNEL_H
#define REMOTEPLAYERCHANNEL_H

#include <QObject>
#include <QTimer>


class RemotePlayerChannel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList userList READ userList NOTIFY userListChanged)

private:
    QStringList m_UserList;
    QStringList m_stillAliveUsers;
    QTimer *m_keepAliveCheckTimer;

public:
    explicit RemotePlayerChannel(QObject *parent = nullptr);

    QStringList userList() const;

protected slots:
    void sendKeepAlive();
    void checkKeepAliveResponses();

public slots:
    bool login(const QString &userName);

    bool logout(const QString &userName);

    bool sendMessage(const QString &user, const QString &msg);

    void keepAliveResponse(const QString &user);

signals:
    void newMessage(QString time, QString user, QString msg);
    void keepAlive();
    void userListChanged();
    void userCountChanged();

};

#endif // REMOTEPLAYERCHANNEL_H
