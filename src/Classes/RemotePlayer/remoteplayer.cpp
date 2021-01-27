#include <QWebSocketServer>
#include "remoteplayer.h"

RemotePlayer::RemotePlayer(QObject *parent) : QObject(parent),
    m_SocketServer(new QWebSocketServer(QStringLiteral("Remote Player Server"), QWebSocketServer::NonSecureMode)),
    m_Connections(new QVector<RemotePlayerTransport*>())
{
    connect(m_SocketServer, &QWebSocketServer::newConnection, this, &RemotePlayer::newConnection);
}

RemotePlayer::~RemotePlayer()
{
    foreach (auto connection, *m_Connections) {
        connection->closeConnection();
    }

    m_Connections->clear();

    stopServer();
}

void RemotePlayer::setPort(const qint32 port)
{
    if (m_Port == port) return;

    m_Port = port;
    emit portChanged();
}

void RemotePlayer::startServer()
{
    if (!m_SocketServer->listen(QHostAddress::Any, 12345)) {
        emit errorWhileStartServer("Failed to open web socket server.");
        return;
    }    
}

void RemotePlayer::stopServer()
{
    if (!m_SocketServer->isListening()) return;

    m_SocketServer->close();
}

void RemotePlayer::broadcastCommand(const QString& command, const QString& argument)
{
    auto message = argument == nullptr || argument.isEmpty() ? command : command + "::" + argument;
    foreach(RemotePlayerTransport* connectionItem, *m_Connections) {
        connectionItem->sendMessage(message);
    }
}

void RemotePlayer::setStarted(const bool started)
{
    m_Started = started;
    emit startedChanged();
}

void RemotePlayer::newConnection()
{
    auto socket = m_SocketServer->nextPendingConnection();
    auto connection = new RemotePlayerTransport(this, socket);
    m_Connections->append(connection);

    connect(connection, &RemotePlayerTransport::simpleCommandReceived, this, &RemotePlayer::simpleCommandReceived);
    connect(connection, &RemotePlayerTransport::forceClosed, this, &RemotePlayer::forceClosed);

    emit newConnectionAccepted(socket->localAddress().toString());
}

void RemotePlayer::simpleCommandReceived(const QString &message, RemotePlayerTransport* connection)
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

void RemotePlayer::forceClosed(const RemotePlayerTransport* connection)
{
    auto localConnection = const_cast<RemotePlayerTransport*>(connection);

    m_Connections->removeOne(localConnection);
}
