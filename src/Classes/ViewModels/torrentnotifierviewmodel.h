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

class TorrentNotifierViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString torrentStreamPath READ torrentStreamPath WRITE setTorrentStreamPath NOTIFY torrentStreamPathChanged)
    Q_PROPERTY(bool activated READ activated NOTIFY activatedChanged)
    Q_PROPERTY(bool removeAllData READ removeAllData WRITE setRemoveAllData NOTIFY removeAllDataChanged)

private:
    QTimer* m_timer { new QTimer(this) };
    QWebSocket* m_webSocket;
    QString m_torrentStreamPath { "" };
    QProcess* m_torrentStreamProcess { nullptr };
    bool m_activated { false };
    bool m_removeAllData { false };

public:
    explicit TorrentNotifierViewModel(QObject *parent = nullptr);

    QString torrentStreamPath() const noexcept { return m_torrentStreamPath; }
    void setTorrentStreamPath(const QString& torrentStreamPath) noexcept;

    bool activated() const noexcept { return m_activated; }

    bool removeAllData() const noexcept { return m_removeAllData; }
    void setRemoveAllData(bool removeAllData) noexcept;

    Q_INVOKABLE void startGetNotifiers(int port);
    Q_INVOKABLE void closeConnectionsAndApplication();
    Q_INVOKABLE void tryStartTorrentStreamApplication();

private slots:
    void triggerNotifier();
    void messageReceived(const QString &message);
    void torrentStreamProcessStarted();
    void socketConnected();
    void socketDisconnected();

signals:
    void torrentFullyDownloaded(int releaseId, const QString& path);
    void torrentStreamStarted();
    void torrentStreamNotConfigured();
    void torrentStreamPathChanged();
    void activatedChanged();
    void removeAllDataChanged();

};

#endif // TORRENTNOTIFIERVIEWMODEL_H
