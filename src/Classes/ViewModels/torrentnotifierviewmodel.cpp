/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2021 Roman Vladimirov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "torrentnotifierviewmodel.h"

TorrentNotifierViewModel::TorrentNotifierViewModel(QObject *parent)
    : QObject{parent}
{
    m_webSocket = new QWebSocket("localhost", QWebSocketProtocol::VersionLatest, this);

    connect(m_timer,&QTimer::timeout, this, &TorrentNotifierViewModel::triggerNotifier);
    connect(m_webSocket,&QWebSocket::textMessageReceived, this, &TorrentNotifierViewModel::messageReceived);
}

void TorrentNotifierViewModel::startGetNotifiers(int port)
{
    m_webSocket->open(QUrl("ws://localhost:" + QString::number(port) + "/ws"));
}

void TorrentNotifierViewModel::stopNotifiers()
{
    m_webSocket->close(QWebSocketProtocol::CloseCode::CloseCodeNormal, "not need");
}

void TorrentNotifierViewModel::triggerNotifier()
{
    if (m_webSocket->state() != QAbstractSocket::ConnectedState) return;

    m_webSocket->sendTextMessage("ds:");
}

void TorrentNotifierViewModel::messageReceived(const QString &message)
{
    if (message.isEmpty()) return;

    auto firstSeparator = message.indexOf(":");
    if (firstSeparator == -1) return;

    auto response = message.midRef(0, firstSeparator);

    if (response == "ds") {
        auto document = QJsonDocument::fromJson(message.mid(3).toUtf8());
        auto object = document.object();
        auto isAll = false;
        QString path = "";
        int identifier = 0;
        if (object.contains("All")) isAll = object.value("All").toBool();
        if (object.contains("Path")) path = object.value("Path").toString();
        if (object.contains("Id")) identifier = object.value("Id").toInt();
        if (identifier > 0 && !path.isEmpty() && isAll) {
            emit torrentFullyDownloaded(identifier, path);
        }
    }
}
