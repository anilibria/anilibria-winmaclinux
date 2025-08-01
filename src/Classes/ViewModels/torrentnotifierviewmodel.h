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


#ifndef TORRENTNOTIFIERVIEWMODEL_H
#define TORRENTNOTIFIERVIEWMODEL_H

#include <QObject>
#include <QTimer>
#include <QWebSocket>
#include <QMap>
#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSharedPointer>
#include <QAbstractSocket>
#include "../Models/downloadedtorrentmodel.h"
#include "../ViewModels/releasesviewmodel.h"
#include "../ListModels/dowloadedtorrentslistmodel.h"

class TorrentNotifierViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString torrentStreamPath READ torrentStreamPath WRITE setTorrentStreamPath NOTIFY torrentStreamPathChanged)
    Q_PROPERTY(bool activated READ activated NOTIFY activatedChanged)
    Q_PROPERTY(bool removeAllData READ removeAllData WRITE setRemoveAllData NOTIFY removeAllDataChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(ReleasesViewModel* releasesViewModel READ releasesViewModel WRITE setReleasesViewModel NOTIFY releasesViewModelChanged)
    Q_PROPERTY(DowloadedTorrentsListModel* torrents READ torrents NOTIFY torrentsChanged)
    Q_PROPERTY(bool needActivateRefreshEvent READ needActivateRefreshEvent NOTIFY needActivateRefreshEventChanged FINAL)
    Q_PROPERTY(int lastRefreshIdentifier READ lastRefreshIdentifier WRITE setLastRefreshIdentifier NOTIFY lastRefreshIdentifierChanged FINAL)
    Q_PROPERTY(bool isCardShowed READ isCardShowed NOTIFY isCardShowedChanged FINAL)
    Q_PROPERTY(DownloadedTorrentModel* cardTorrent READ cardTorrent NOTIFY cardTorrentChanged FINAL)
    Q_PROPERTY(QVariantList cardTorrentFiles READ cardTorrentFiles NOTIFY cardTorrentFilesChanged FINAL)
    Q_PROPERTY(int cardReleaseId READ cardReleaseId NOTIFY cardReleaseIdChanged FINAL)
    Q_PROPERTY(QString cardReleaseName READ cardReleaseName NOTIFY cardReleaseNameChanged FINAL)
    Q_PROPERTY(QString cardDownloadStatus READ cardDownloadStatus NOTIFY cardDownloadStatusChanged FINAL)
    Q_PROPERTY(QString cardDownloadSize READ cardDownloadSize NOTIFY cardDownloadSizeChanged FINAL)
    Q_PROPERTY(QString cardDownloadFileStatus READ cardDownloadFileStatus NOTIFY cardDownloadFileStatusChanged FINAL)

private:
    QWebSocket* m_webSocket;
    QString m_torrentStreamPath { "" };
    QProcess* m_torrentStreamProcess { nullptr };
    bool m_activated { false };
    bool m_removeAllData { false };
    bool m_dataRemoved { false };
    QSharedPointer<QList<DownloadedTorrentModel*>> m_downloadedTorrents { new QList<DownloadedTorrentModel*>() };
    QNetworkAccessManager* m_manager { new QNetworkAccessManager(this) };
    int m_port { 0 };
    ReleasesViewModel* m_releasesViewModel { nullptr };
    DowloadedTorrentsListModel* m_torrents { new DowloadedTorrentsListModel(this) };
    bool m_needActivateRefreshEvent { false };
    int m_lastRefreshIdentifier { -1 };
    int m_howMuchTimesTryConnect { 0 };
    bool m_isCardShowed { false };
    DownloadedTorrentModel* m_cardTorrent { nullptr };
    QVariantList m_cardTorrentFiles { QVariantList() };

public:
    explicit TorrentNotifierViewModel(QObject *parent = nullptr);

    QString torrentStreamPath() const noexcept { return m_torrentStreamPath; }
    void setTorrentStreamPath(const QString& torrentStreamPath) noexcept;

    bool activated() const noexcept { return m_activated; }

    bool removeAllData() const noexcept { return m_removeAllData; }
    void setRemoveAllData(bool removeAllData) noexcept;

    int port() const noexcept { return m_port; }
    void setPort(int port) noexcept;

    ReleasesViewModel* releasesViewModel() const noexcept { return m_releasesViewModel; }
    void setReleasesViewModel(const ReleasesViewModel* viewmodel) noexcept;

    DowloadedTorrentsListModel* torrents() const noexcept { return m_torrents; }

    bool needActivateRefreshEvent() const noexcept { return m_needActivateRefreshEvent; }

    int isCardShowed() const noexcept { return m_isCardShowed; }

    DownloadedTorrentModel* cardTorrent() const noexcept { return m_cardTorrent; }

    int cardReleaseId() const noexcept { return m_cardTorrent != nullptr ? m_cardTorrent->releaseId() : 0; }

    QString cardReleaseName() const noexcept { return m_cardTorrent != nullptr ? m_cardTorrent->title() : ""; }

    QString cardDownloadStatus() const noexcept { return m_cardTorrent != nullptr ? (m_cardTorrent->allDownloaded() ? "Скачан полностью" : "Частично скачан") : ""; }

    QVariantList cardTorrentFiles() const noexcept { return m_cardTorrentFiles; }

    QString cardDownloadSize() const noexcept { return m_cardTorrent != nullptr ? getReadableSize(m_cardTorrent->getSizeAllFiles()) : ""; }

    QString getDownloadedPath(const QString& url, int fileIndex) const noexcept;

    QString cardDownloadFileStatus() const noexcept { return getCardDownloadFileStatus(); }

    int lastRefreshIdentifier() const noexcept { return m_lastRefreshIdentifier; }
    void setLastRefreshIdentifier(int lastRefreshIdentifier) noexcept;

    Q_INVOKABLE void startGetNotifiers();
    Q_INVOKABLE void closeConnectionsAndApplication();
    Q_INVOKABLE void tryStartTorrentStreamApplication();
    Q_INVOKABLE void startGetTorrentData(bool needNotify);
    Q_INVOKABLE void clearAllData();
    Q_INVOKABLE void watchDownloadedTorrents(int index) noexcept;
    Q_INVOKABLE void clearOnlyTorrent(const QString& path) noexcept;
    Q_INVOKABLE void clearTorrentAndData(const QString& path) noexcept;
    Q_INVOKABLE void removeRedundant() noexcept;
    Q_INVOKABLE void showCard(int index) noexcept;
    Q_INVOKABLE void closeCard() noexcept;

private:
    void getTorrentData() const noexcept;
    QString getReadableSize(int64_t size) const noexcept;
    QString getCardDownloadFileStatus() const noexcept;

private slots:
    void triggerNotifier();
    void messageReceived(const QString &message);
    void socketConnected();
    void socketDisconnected();
    void errorSocket(QAbstractSocket::SocketError error);
    void requestResponse(QNetworkReply* reply);
    void makeConnectToNotifiers();

signals:
    void torrentFullyDownloaded(const QString& releaseName);
    void torrentStreamStarted();
    void torrentStreamNotConfigured();
    void torrentStreamPathChanged();
    void activatedChanged();
    void removeAllDataChanged();
    void portChanged();
    void releasesViewModelChanged();
    void torrentsChanged();
    void prepareWatchTorrentFiles(QStringList files, int releaseId);
    void torrentsRefreshed();
    void needActivateRefreshEventChanged();
    void lastRefreshIdentifierChanged();
    void isCardShowedChanged();
    void cardTorrentChanged();
    void cardTorrentFilesChanged();
    void cardReleaseIdChanged();
    void cardReleaseNameChanged();
    void cardDownloadStatusChanged();
    void cardDownloadSizeChanged();
    void cardDownloadFileStatusChanged();

};

#endif // TORRENTNOTIFIERVIEWMODEL_H
