#include "remoteplayerclientwrapper.h"

#include <QWebSocketServer>

RemotePlayerClientWrapper::RemotePlayerClientWrapper(QWebSocketServer *server, QObject *parent) : QObject(parent), m_Server(server)
{
    connect(server, &QWebSocketServer::newConnection, this, &RemotePlayerClientWrapper::handleNewConnection);
}

void RemotePlayerClientWrapper::handleNewConnection()
{
    emit clientConnected(new RemotePlayerTransport(this, m_Server->nextPendingConnection()));
}

