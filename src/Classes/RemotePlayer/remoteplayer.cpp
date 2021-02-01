#include <QWebSocketServer>
#include <QMutexLocker>
#include "remoteplayer.h"
#include <QDebug>

RemotePlayer::RemotePlayer(QObject *parent) : QObject(parent),
    m_SocketServer(new QWebSocketServer(QStringLiteral("Remote Player Server"), QWebSocketServer::NonSecureMode)),
    m_Connections(new QVector<RemotePlayerTransport*>()),
    m_Port(12345),
    m_Started(false),
    m_GetIdMutex(),
    m_MaximumIdentifier(0)
{
    connect(m_SocketServer, &QWebSocketServer::newConnection, this, &RemotePlayer::newConnection);
}

RemotePlayer::~RemotePlayer()
{
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
    if (!m_SocketServer->listen(QHostAddress::Any, m_Port)) {
        emit errorWhileStartServer("Failed to open web socket server.");
        return;
    }

    setStarted(true);
}

void RemotePlayer::stopServer() noexcept
{
    if (!m_SocketServer->isListening()) return;

    foreach (auto connection, *m_Connections) {
        connection->closeConnection();
    }

    m_Connections->clear();

    m_SocketServer->close();

    setStarted(false);
}

void RemotePlayer::broadcastCommand(const QString& command, const QString& argument)
{
    auto message = argument == nullptr || argument.isEmpty() ? command : command + "::" + argument;
    foreach(RemotePlayerTransport* connectionItem, *m_Connections) {
        connectionItem->sendMessage(message);
    }
}

void RemotePlayer::sendCommandToUser(const unsigned int id, const QString &command, const QString &argument)
{
    auto findedConnection = std::find_if(
        m_Connections->begin(),
        m_Connections->end(),
        [id](RemotePlayerTransport* connection) -> bool {
            return connection->id() == id;
        }
    );
    if (findedConnection == m_Connections->end()) return;

    auto connection = *findedConnection;

    auto message = argument == nullptr || argument.isEmpty() ? command : command + "::" + argument;
    connection->sendMessage(message);
}

void RemotePlayer::setStarted(const bool started)
{
    m_Started = started;
    emit startedChanged();
}

unsigned int RemotePlayer::getId()
{
    QMutexLocker locker(&m_GetIdMutex);

    return m_MaximumIdentifier + 1;
}

void RemotePlayer::newConnection()
{
    auto socket = m_SocketServer->nextPendingConnection();
    auto id = getId();
    auto connection = new RemotePlayerTransport(this, socket, id);
    m_Connections->append(connection);

    connect(connection, &RemotePlayerTransport::simpleCommandReceived, this, &RemotePlayer::simpleCommandReceived);
    connect(connection, &RemotePlayerTransport::forceClosed, this, &RemotePlayer::forceClosed);

    emit newConnectionAccepted(socket->localAddress().toString());
}

void RemotePlayer::simpleCommandReceived(const QString &message, RemotePlayerTransport* connection)
{
    if (message == "ping") connection->sendMessage("pong");

    if (message == "getcurrentvideosource") {
        emit receiveCommand(connection->id(), "getcurrentvideosource", "");
        return;
    }

    if (message.startsWith("videosourcechanged::") || message.startsWith("volumechanged::") || message.startsWith("positionchanged::")) {
        foreach(RemotePlayerTransport* connectionItem, *m_Connections) {
            if (connectionItem == connection) continue;

            connectionItem->sendMessage(message);
        }
    }
}

void RemotePlayer::forceClosed(const RemotePlayerTransport* connection)
{
    auto localConnection = const_cast<RemotePlayerTransport*>(connection);

    m_Connections->removeOne(localConnection);
}
