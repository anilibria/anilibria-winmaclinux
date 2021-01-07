#ifndef REMOTEPLAYERTRANSPORT_H
#define REMOTEPLAYERTRANSPORT_H

#include <QObject>
#include <QWebChannelAbstractTransport>
#include <QWebSocket>

class RemotePlayerTransport : public QObject
{
    Q_OBJECT

private:
    QWebSocket *m_WebSocket;

public:
    explicit RemotePlayerTransport(QObject *parent = nullptr, QWebSocket* socket = nullptr);
    virtual ~RemotePlayerTransport();

    void sendMessage(const QString& message);

private slots:
    void textMessageReceived(const QString& message);

signals:
    void messageReceived(const QString& message, RemotePlayerTransport* connection);

};

#endif // REMOTEPLAYERTRANSPORT_H
