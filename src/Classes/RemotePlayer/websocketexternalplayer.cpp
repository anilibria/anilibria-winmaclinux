#include "websocketexternalplayer.h"

WebSocketExternalPlayer::WebSocketExternalPlayer(QObject *parent)
    : ExternalPlayerBase{parent}
{
    m_socket->open(QUrl(""));
    connect(m_socket, &QWebSocket::textMessageReceived, this, &WebSocketExternalPlayer::onTextMessageReceived);
    //connect(&m_webSocket, &QWebSocket::connected, this, &EchoClient::onConnected);
}

void WebSocketExternalPlayer::trySetNewState(const QString &state)
{
    sendCommand("st", state);
}

void WebSocketExternalPlayer::trySetNewVolume(int volume)
{
    sendCommand("vm", QString::number(volume));
}

void WebSocketExternalPlayer::trySetSource(const QString &path)
{
    sendCommand("sc", path);
}

void WebSocketExternalPlayer::trySetSeek(int position)
{
    sendCommand("sk", QString::number(position));
}

void WebSocketExternalPlayer::tryMuted(bool mute)
{
    sendCommand("mt", mute ? "true" : "false");
}

void WebSocketExternalPlayer::sendCommand(const QString &command, const QString &parameter)
{
    auto message = command + ":" + parameter;
    m_socket->sendTextMessage(message);
}

void WebSocketExternalPlayer::onTextMessageReceived(const QString &message)
{
    auto parts = message.split(":");
    if (parts.count() != 2) return;

    auto command = parts.first();
    auto parameter = parts.last();

    if (command == "svm") emit volumeChanged(parameter.toInt());
    if (command == "sst") emit stateChanged(parameter);
    if (command == "ssk") emit positionChanged(parameter.toInt());
}
