#ifndef REMOTEPLAYERTRANSPORT_H
#define REMOTEPLAYERTRANSPORT_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QWebSocket>

class RemotePlayerTransport : public QObject
{
    Q_OBJECT

private:
    QWebSocket* m_WebSocket;
    QTimer* m_keepAliveCheckTimer;
    QDateTime* m_DateTime;

public:
    explicit RemotePlayerTransport(QObject *parent = nullptr, QWebSocket* socket = nullptr);
    virtual ~RemotePlayerTransport();

    void sendMessage(const QString& message);
    void closeConnection();

private slots:
    void textMessageReceived(const QString& message);
    void keepAliveTimer();

signals:
    void simpleCommandReceived(const QString& message, RemotePlayerTransport* connection);
    void forceClosed(const RemotePlayerTransport* connection);

};

#endif // REMOTEPLAYERTRANSPORT_H
