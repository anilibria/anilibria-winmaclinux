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
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include "torrentnotifierviewmodel.h"

TorrentNotifierViewModel::TorrentNotifierViewModel(QObject *parent)
    : QObject{parent}
{
    m_webSocket = new QWebSocket("localhost", QWebSocketProtocol::VersionLatest, this);

    connect(m_timer,&QTimer::timeout, this, &TorrentNotifierViewModel::triggerNotifier);
    connect(m_webSocket,&QWebSocket::textMessageReceived, this, &TorrentNotifierViewModel::messageReceived);
    connect(m_webSocket,&QWebSocket::connected, this, &TorrentNotifierViewModel::socketConnected);
    connect(m_webSocket,&QWebSocket::disconnected, this, &TorrentNotifierViewModel::socketConnected);
}

void TorrentNotifierViewModel::setTorrentStreamPath(const QString &torrentStreamPath) noexcept
{
    if (m_torrentStreamPath == torrentStreamPath) return;

    m_torrentStreamPath = torrentStreamPath;
    emit torrentStreamPathChanged();
}

void TorrentNotifierViewModel::setRemoveAllData(bool removeAllData) noexcept
{
    if (m_removeAllData == removeAllData) return;

    m_removeAllData = removeAllData;
    emit removeAllDataChanged();
}

void TorrentNotifierViewModel::startGetNotifiers(int port)
{
    m_webSocket->open(QUrl("ws://localhost:" + QString::number(port) + "/ws"));
}

void TorrentNotifierViewModel::closeConnectionsAndApplication()
{
    if (!m_activated) return;

    if (m_webSocket->state() == QAbstractSocket::SocketState::ConnectedState) m_webSocket->close(QWebSocketProtocol::CloseCode::CloseCodeNormal, "not need");

    if (m_torrentStreamProcess != nullptr) m_torrentStreamProcess->kill();
}

void TorrentNotifierViewModel::tryStartTorrentStreamApplication()
{
    if (m_torrentStreamPath.isEmpty()) return;

    QFileInfo fileInfo(m_torrentStreamPath);
    if (!QFile::exists(fileInfo.absoluteFilePath())) {
        qInfo() << "TorrentStream path not configured";
        emit torrentStreamNotConfigured();
        return;
    }

    QStringList arguments;
#ifdef Q_OS_WIN
    arguments.append("-noconsole");
#endif

    m_torrentStreamProcess = new QProcess(this);
    m_torrentStreamProcess->start(fileInfo.absoluteFilePath(), arguments);

    connect(m_torrentStreamProcess, &QProcess::started, this, &TorrentNotifierViewModel::torrentStreamProcessStarted);
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

void TorrentNotifierViewModel::torrentStreamProcessStarted()
{
    qInfo() << "TorrentStream started from application";
    m_activated = true;
    emit activatedChanged();
    emit torrentStreamStarted();
}

void TorrentNotifierViewModel::socketConnected()
{
    if (m_activated) return;

    m_activated = true;
    emit activatedChanged();

    qInfo() << "TorrentStream socket connected";

    if (m_removeAllData) {
        //TODO: http://localhost:X/clearall for removing all data
    }
}

void TorrentNotifierViewModel::socketDisconnected()
{
    if (!m_activated) return;

    m_activated = false;
    emit activatedChanged();

    qInfo() << "TorrentStream socket disconnected";
}

