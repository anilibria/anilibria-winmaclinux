#ifndef VIDEOPROXYSERVER_H
#define VIDEOPROXYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QList>
#include <QByteArray>
#include <QTcpSocket>
#include <QSslSocket>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class VideoProxyServer : public QTcpServer
{
    Q_OBJECT

private:
    QNetworkAccessManager* m_manager { new QNetworkAccessManager() };
    QMap<int, QTcpSocket*> m_sockets { QMap<int, QTcpSocket*>() };

protected:
    void incomingConnection(qintptr socketDescriptor) override;

public:
    explicit VideoProxyServer(QObject *parent = nullptr);

private:
    QByteArray getRoute(QByteArray bytes);
    void closeSocket(QTcpSocket* socket);
    QByteArray readAllAvailableBytesFromSocket(QTcpSocket * socket);
    void getDataFromUrl(QString url, int socketDescriptor);

private slots:
    void finishedResponse(QNetworkReply* reply);

signals:

};

#endif // VIDEOPROXYSERVER_H
