/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2020 Roman Vladimirov

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

#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QTimer>
#include <QDebug>
#include <QVector>

class DownloadManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString displayBytesInSeconds READ displayBytesInSeconds NOTIFY displayBytesInSecondsChanged)
    Q_PROPERTY(QUrl destination READ destination WRITE setDestination NOTIFY destinationChanged)

private:
    QNetworkAccessManager* m_CurrentAccessManager;
    QNetworkReply* m_CurrentNetworkReply;
    QTimer* m_DownloadSpeedTimer;
    QUrl m_Url;
    bool m_Running;
    qreal m_Progress;
    QUrl m_Destination;
    QString m_DisplayBytesInSeconds;
    qint64 m_BytesInSeconds;

public:
    explicit DownloadManager(QObject *parent = nullptr);

    QUrl url() const { return m_Url; }
    bool running() const { return m_Running; }
    qreal progress() const { return m_Progress; }
    QUrl destination() const { return m_Destination; }
    QString displayBytesInSeconds() const { return m_DisplayBytesInSeconds; }

    void setUrl(QUrl url) noexcept;
    void setDestination(QUrl destination) noexcept;

private:
    void setDisplayBytesInSeconds(QString displayBytesInSeconds);
    void setRunning(bool running);
    void setProgress(qreal progress);

signals:
    void urlChanged(QUrl url);
    void seriaDownloaded(int id, int seriaId);
    void runningChanged(bool running);
    void progressChanged(qreal progress);
    void destinationChanged(QUrl destination);
    void displayBytesInSecondsChanged(QString bytesInSeconds);

    void started();
    void finished();
    void update(int kiloBytesReceived, int kiloBytesTotal);
    void error(int errorCode, QString errorString);

public slots:
    void start();
    void stop();

private slots:
    void onFinished();
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onTimerTimeout();

};

#endif // DOWNLOADMANAGER_H
