#include "remoteplayertransport.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWebSocket>

RemotePlayerTransport::RemotePlayerTransport(QObject *parent, QWebSocket* socket) : QObject(parent),
    m_WebSocket(socket),
    m_keepAliveCheckTimer(new QTimer(this)),
    m_DateTime(new QDateTime())
{
    m_keepAliveCheckTimer->setInterval(15000);
    m_keepAliveCheckTimer->start();

    connect(m_keepAliveCheckTimer, &QTimer::timeout, this, &RemotePlayerTransport::keepAliveTimer);
    connect(socket, &QWebSocket::textMessageReceived, this, &RemotePlayerTransport::textMessageReceived);
    connect(socket, &QWebSocket::disconnected, this, &RemotePlayerTransport::deleteLater);
}

RemotePlayerTransport::~RemotePlayerTransport()
{
    m_WebSocket->deleteLater();
}

void RemotePlayerTransport::sendMessage(const QString& message)
{
    m_WebSocket->sendTextMessage(message);
}

void RemotePlayerTransport::closeConnection()
{
    m_WebSocket->close(QWebSocketProtocol::CloseCodeNormal, "Server is shutdown");
}

void RemotePlayerTransport::textMessageReceived(const QString& messageData)
{
    if (messageData == "pong")
    emit simpleCommandReceived(messageData, this);
}

void RemotePlayerTransport::keepAliveTimer()
{
    auto now = QDateTime::currentDateTime();
    auto difference = m_DateTime->msecsTo(now);

    if (difference > 20000) {
        m_WebSocket->close(QWebSocketProtocol::CloseCodeNormal, "Ping/Pong session is expire");
    }
}
