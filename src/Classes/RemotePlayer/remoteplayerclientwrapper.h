#ifndef REMOTEPLAYERCLIENTWRAPPER_H
#define REMOTEPLAYERCLIENTWRAPPER_H

#include <QObject>
#include <QWebSocketServer>
#include "./remoteplayertransport.h"

class RemotePlayerClientWrapper : public QObject
{
    Q_OBJECT
public:
    explicit RemotePlayerClientWrapper(QWebSocketServer *server, QObject *parent = nullptr);

signals:
    void clientConnected(RemotePlayerTransport *client);
private slots:
    void handleNewConnection();
private:
    QWebSocketServer *m_Server;

};

#endif // REMOTEPLAYERCLIENTWRAPPER_H
