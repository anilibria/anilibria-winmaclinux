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

    m_torrents->setup(m_downloadedTorrents);

    connect(m_webSocket,&QWebSocket::textMessageReceived, this, &TorrentNotifierViewModel::messageReceived);
    connect(m_webSocket,&QWebSocket::connected, this, &TorrentNotifierViewModel::socketConnected);
    connect(m_webSocket,&QWebSocket::disconnected, this, &TorrentNotifierViewModel::socketDisconnected);
    connect(m_manager,&QNetworkAccessManager::finished, this, &TorrentNotifierViewModel::requestResponse);
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

void TorrentNotifierViewModel::setPort(int port) noexcept
{
    if (m_port == port) return;

    m_port = port;
    emit portChanged();
}

void TorrentNotifierViewModel::setReleasesViewModel(const ReleasesViewModel *viewmodel) noexcept
{
    if (m_releasesViewModel == viewmodel) return;

    m_releasesViewModel = const_cast<ReleasesViewModel*>(viewmodel);
    emit releasesViewModelChanged();
}

void TorrentNotifierViewModel::startGetNotifiers()
{
    m_webSocket->open(QUrl("ws://localhost:" + QString::number(m_port) + "/ws"));
}

void TorrentNotifierViewModel::closeConnectionsAndApplication()
{
    if (!m_activated) return;

    if (m_webSocket->state() == QAbstractSocket::SocketState::ConnectedState) m_webSocket->close(QWebSocketProtocol::CloseCode::CloseCodeNormal, "not need");

    if (m_torrentStreamProcess != nullptr) m_torrentStreamProcess->kill();
}

void TorrentNotifierViewModel::tryStartTorrentStreamApplication()
{
    if (!m_torrentStreamPath.isEmpty()) {
        QFileInfo fileInfo(m_torrentStreamPath);
        auto absolutePath = fileInfo.absoluteFilePath();
        if (!QFile::exists(absolutePath)) {
            qInfo() << "TorrentStream path not correct";
            emit torrentStreamNotConfigured();
            return;
        }

        QStringList arguments;
#ifdef Q_OS_WIN
        arguments.append("-noconsole");
#endif

        m_torrentStreamProcess = new QProcess(this);
        m_torrentStreamProcess->setWorkingDirectory(fileInfo.absolutePath());
        m_torrentStreamProcess->start(absolutePath, arguments);

        if (!m_torrentStreamProcess->waitForStarted(10000)) {
            qInfo () << "Failing to start TorrentStream: " << m_torrentStreamProcess->errorString();
            return;
        }

        emit torrentStreamStarted();
    } else {
        qInfo() << "TorrentStream path not configured";
        emit torrentStreamNotConfigured();
    }
}

void TorrentNotifierViewModel::startGetTorrentData()
{
    getTorrentData();
}

void TorrentNotifierViewModel::clearAllData()
{
    QUrl url("http://localhost:" + QString::number(m_port) + "/clearall");
    QNetworkRequest request(url);
    m_manager->get(request);
}

void TorrentNotifierViewModel::watchDownloadedTorrents(int index) noexcept
{
    if (index >= m_downloadedTorrents->count()) return;

    auto torrent = m_downloadedTorrents->at(index);

    emit prepareWatchTorrentFiles(torrent->getFiles(), torrent->releaseId());
}

void TorrentNotifierViewModel::clearOnlyTorrent(const QString &path) noexcept
{
    QUrl url("http://localhost:" + QString::number(m_port) + "/clearonlytorrent?path=" + path);
    QNetworkRequest request(url);
    m_manager->get(request);
}

void TorrentNotifierViewModel::clearTorrentAndData(const QString &path) noexcept
{
    QUrl url("http://localhost:" + QString::number(m_port) + "/cleartorrentanddata?path=" + path);
    QNetworkRequest request(url);
    m_manager->get(request);
}

void TorrentNotifierViewModel::getTorrentData() const noexcept
{
    QUrl url("http://localhost:" + QString::number(m_port) + "/torrents");
    QNetworkRequest request(url);
    auto reply = m_manager->get(request);
    reply->setProperty("responsecode", "torrentsData");
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
            auto release = m_releasesViewModel->getReleaseById(identifier);
            if (release != nullptr) {
                emit torrentFullyDownloaded(release->title());
            }
        }
    }
    if (response == "nt") getTorrentData();
    if (response == "dt") getTorrentData();
}

void TorrentNotifierViewModel::socketConnected()
{
    if (m_activated) return;

    m_activated = true;
    emit activatedChanged();

    qInfo() << "TorrentStream socket connected";

    if (m_removeAllData && !m_dataRemoved) {
        m_dataRemoved = true;
        clearAllData();
        qInfo() << "TorrentStream clear downloaded data";
    }
}

void TorrentNotifierViewModel::socketDisconnected()
{
    if (!m_activated) return;

    m_activated = false;
    emit activatedChanged();

    qInfo() << "TorrentStream socket disconnected";
}

void TorrentNotifierViewModel::requestResponse(QNetworkReply *reply)
{
    if (m_releasesViewModel == nullptr) return;
    if (m_downloadedTorrents == nullptr) return;

    auto responseCode = reply->property("responsecode");
    if (responseCode.isNull()) return;

    auto response = responseCode.toString();

    if (response == "torrentsData") {
        foreach (auto downloadedTorrent, *m_downloadedTorrents) downloadedTorrent->resetData();
        m_downloadedTorrents->clear();

        auto json = reply->readAll();

        auto jsonDocument = QJsonDocument::fromJson(json);
        auto array = jsonDocument.array();
        foreach (auto item, array) {
            auto torrentItem = item.toObject();
            auto releaseId = torrentItem.value("identifier").toInt();
            if (releaseId == 0) continue;
            auto releaseItem = m_releasesViewModel->getReleaseById(releaseId);
            if (releaseItem == nullptr) continue;

            auto downloadedItem = new DownloadedTorrentModel();
            downloadedItem->setReleaseId(releaseId);
            downloadedItem->setDownloadPath(torrentItem.value("downloadPath").toString());
            downloadedItem->setAllDownloaded(torrentItem.value("allDownloaded").toBool());
            auto files = torrentItem.value("files").toArray();
            foreach (auto file, files) {
                auto fileObject = file.toObject();
                downloadedItem->addFile(
                    fileObject.value("isDownloaded").toBool(),
                    fileObject.value("percentComplete").toInt(),
                    fileObject.value("downloadedPath").toString()
                );
            }
            downloadedItem->setTitle(releaseItem->title());
            downloadedItem->setPoster(releaseItem->poster());
            //TODO: torrent title
            //downloadedItem->setTorrentPoster(releaseItem->torrents())
            m_downloadedTorrents->append(downloadedItem);
        }

        m_torrents->refresh();
    }
}

