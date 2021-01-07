#include "remoteplayertransport.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWebSocket>

RemotePlayerTransport::RemotePlayerTransport(QObject *parent, QWebSocket* socket) : QObject(parent), m_WebSocket(socket)
{
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

void RemotePlayerTransport::textMessageReceived(const QString &messageData)
{
    QJsonParseError error;
    QJsonDocument message = QJsonDocument::fromJson(messageData.toUtf8(), &error);
    if (error.error) {
        qWarning() << "Failed to parse text message as JSON object:" << messageData
                   << "Error is:" << error.errorString();
        return;
    } else if (!message.isObject()) {
        qWarning() << "Received JSON message that is not an object: " << messageData;
        return;
    }
    emit messageReceived(messageData, this);//message.object());
}
