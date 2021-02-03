#include "remoteplayertransport.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWebSocket>

RemotePlayerTransport::RemotePlayerTransport(QObject *parent, QWebSocket* socket, unsigned int id) : QObject(parent),
    m_WebSocket(socket),
    m_keepAliveCheckTimer(new QTimer(this)),
    m_DateTime(new QDateTime()),
    m_Id(id)
{
    m_keepAliveCheckTimer->setInterval(10000);
    m_keepAliveCheckTimer->start();

    connect(m_keepAliveCheckTimer, &QTimer::timeout, this, &RemotePlayerTransport::keepAliveTimer);
    connect(socket, &QWebSocket::textMessageReceived, this, &RemotePlayerTransport::textMessageReceived);
    connect(socket, &QWebSocket::disconnected, this, &RemotePlayerTransport::disconnect);
}

RemotePlayerTransport::~RemotePlayerTransport()
{
    m_keepAliveCheckTimer->stop();
    m_WebSocket->deleteLater();
}

void RemotePlayerTransport::sendMessage(const QString& message)
{
    m_WebSocket->sendTextMessage(message);
}

void RemotePlayerTransport::closeConnection()
{
    m_keepAliveCheckTimer->stop();
    m_WebSocket->close(QWebSocketProtocol::CloseCodeNormal, "Server is shutdown");
    emit forceClosed(this);
}

void RemotePlayerTransport::textMessageReceived(const QString& messageData)
{
    if (messageData.startsWith("{")) {
        //TODO: support json responses!
        return;
    }

    emit simpleCommandReceived(messageData, this);
}

void RemotePlayerTransport::keepAliveTimer()
{
    auto now = QDateTime::currentDateTime();
    auto difference = m_DateTime->msecsTo(now);

    if (difference > 15000) {
        m_WebSocket->close(QWebSocketProtocol::CloseCodeNormal, "Ping/Pong session is expire");
    }
}

void RemotePlayerTransport::disconnect()
{
    m_keepAliveCheckTimer->stop();
    m_WebSocket->deleteLater();
    emit forceClosed(this);
}
