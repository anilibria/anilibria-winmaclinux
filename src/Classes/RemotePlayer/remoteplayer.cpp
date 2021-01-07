#include <QWebChannel>
#include <QWebSocketServer>
#include "remoteplayerchannel.h"
#include "remoteplayer.h"
#include "remoteplayerclientwrapper.h"

RemotePlayer::RemotePlayer(QObject *parent) : QObject(parent),
    m_SocketServer(new QWebSocketServer(QStringLiteral("Remote Player Server"), QWebSocketServer::NonSecureMode)),
    m_Connections(new QVector<RemotePlayerTransport*>())
{
    connect(m_SocketServer, &QWebSocketServer::newConnection, this, &RemotePlayer::newConnection);
}

void RemotePlayer::startServer()
{
    if (!m_SocketServer->listen(QHostAddress::LocalHost, 12345)) {
        emit errorWhileStartServer("Failed to open web socket server.");
        return;
    }

    /*m_RemotePlayerClientWrapper = new RemotePlayerClientWrapper(m_SocketServer);

    m_QWebChannel = new QWebChannel();
    QObject::connect(m_RemotePlayerClientWrapper, &RemotePlayerClientWrapper::clientConnected, m_QWebChannel, &QWebChannel::connectTo);

    m_RemotePlayerChannel = new RemotePlayerChannel(this);
    m_QWebChannel->registerObject(QStringLiteral("playerserver"), m_RemotePlayerChannel);*/
}

void RemotePlayer::stopServer()
{
    if (!m_SocketServer->isListening()) return;

    m_SocketServer->close();
}

void RemotePlayer::newConnection()
{
    auto socket = m_SocketServer->nextPendingConnection();
    auto connection = new RemotePlayerTransport(socket);
    m_Connections->append(connection);

    connect(connection, &RemotePlayerTransport::messageReceived, this, &RemotePlayer::messageReceived);

    emit newConnectionAccepted(socket->localAddress().toString());
}

void RemotePlayer::messageReceived(const QString &message, RemotePlayerTransport *connection)
{
    if (message == "ping") connection->sendMessage("pong");

    if (message.startsWith("changeposition::")) {
        auto newPosition = message.midRef(16).toInt();
        foreach(RemotePlayerTransport* connectionItem, *m_Connections) {
            if (connectionItem == connection) continue;

            connectionItem->sendMessage("positionchanged::" + QString::number(newPosition));
        }
    }

    if (message.startsWith("changevolume::")) {
        auto newPosition = message.midRef(14).toInt();
        foreach(RemotePlayerTransport* connectionItem, *m_Connections) {
            if (connectionItem == connection) continue;

            connectionItem->sendMessage("volumechanged::" + QString::number(newPosition));
        }
    }

}
