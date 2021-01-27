#ifndef REMOTEPLAYER_H
#define REMOTEPLAYER_H

#include <QObject>
#include <QWebSocketServer>
#include "remoteplayertransport.h"

class RemotePlayer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qint32 port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(bool started READ started NOTIFY startedChanged)

private:
    QWebSocketServer* m_SocketServer;
    QVector<RemotePlayerTransport*>* m_Connections;
    qint32 m_Port;
    bool m_Started;

public:
    explicit RemotePlayer(QObject *parent = nullptr);
    ~RemotePlayer();

    qint32 port() const { return m_Port; }
    bool started() const { return m_Started; }

    void setPort(const qint32 port);

    Q_INVOKABLE void startServer();
    Q_INVOKABLE void stopServer();
    Q_INVOKABLE void broadcastCommand(const QString& command, const QString& argument);

private:
    void setStarted(const bool started);

private slots:
    void newConnection();
    void simpleCommandReceived(const QString& message, RemotePlayerTransport* connection);
    void forceClosed(const RemotePlayerTransport* connection);

signals:
    void errorWhileStartServer(const QString& message);
    void newConnectionAccepted(const QString& address);
    void portChanged();
    void startedChanged();

};

#endif // REMOTEPLAYER_H
