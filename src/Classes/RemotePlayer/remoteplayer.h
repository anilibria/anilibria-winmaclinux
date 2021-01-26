#ifndef REMOTEPLAYER_H
#define REMOTEPLAYER_H

#include <QObject>
#include <QWebSocketServer>
#include "remoteplayerchannel.h"
#include "remoteplayerclientwrapper.h"

class RemotePlayer : public QObject
{
    Q_OBJECT
private:
    QWebSocketServer* m_SocketServer;
    RemotePlayerChannel* m_RemotePlayerChannel;
    RemotePlayerClientWrapper* m_RemotePlayerClientWrapper;
    QVector<RemotePlayerTransport*>* m_Connections;

public:
    explicit RemotePlayer(QObject *parent = nullptr);

    Q_INVOKABLE void startServer();
    Q_INVOKABLE void stopServer();

private slots:
    void newConnection();
    void simpleCommandReceived(const QString& message, RemotePlayerTransport* connection);

signals:
    void errorWhileStartServer(const QString& message);
    void newConnectionAccepted(const QString& address);

};

#endif // REMOTEPLAYER_H
