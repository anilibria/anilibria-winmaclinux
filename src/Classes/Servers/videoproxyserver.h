#ifndef VIDEOPROXYSERVER_H
#define VIDEOPROXYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QList>
#include <QByteArray>
#include <QTcpSocket>
#include <QSslSocket>

class VideoProxyServer : public QTcpServer
{
    Q_OBJECT

protected:
    void incomingConnection(qintptr socketDescriptor) override;

public:
    explicit VideoProxyServer(QObject *parent = nullptr);

private:
    void processSocket(int socket);
    QByteArray getRoute(QByteArray bytes);
    void closeSocket(QTcpSocket* socket);
    QByteArray readAllAvailableBytesFromSocket(QTcpSocket * socket);

signals:

};

#endif // VIDEOPROXYSERVER_H
