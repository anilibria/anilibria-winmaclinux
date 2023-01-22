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

class TorrentNotifierViewModel : public QObject
{
    Q_OBJECT

private:
    QTimer* m_timer { new QTimer(this) };
    QWebSocket* m_webSocket;
    QMap<QString, bool> m_downloadedStatuses { QMap<QString, bool>() };

public:
    explicit TorrentNotifierViewModel(QObject *parent = nullptr);

    Q_INVOKABLE void startGetNotifiers(int port);
    Q_INVOKABLE void stopNotifiers();

private slots:
    void triggerNotifier();
    void messageReceived(const QString &message);

signals:
    void torrentFullyDownloaded(int releaseId);

};

#endif // TORRENTNOTIFIERVIEWMODEL_H
