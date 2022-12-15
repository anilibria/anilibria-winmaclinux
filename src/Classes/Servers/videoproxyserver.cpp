#include <QtConcurrent>
#include <QNetworkRequest>
#include "videoproxyserver.h"
#include "../../globalconstants.h"

void VideoProxyServer::incomingConnection(qintptr socketDescriptor)
{
    auto tcpSocket = new QTcpSocket();
    if (!tcpSocket->setSocketDescriptor(socketDescriptor)) {
        qWarning() << "Error while try read socket descriptor in first part";
        return;
    }

    auto data = readAllAvailableBytesFromSocket(tcpSocket);
    auto routeData = getRoute(data);
    if (routeData.isEmpty()) {
        qWarning() << "Error while try read request first line from socket ";
        closeSocket(tcpSocket);
        tcpSocket = nullptr;
        return;
    }

    m_sockets.insert(socketDescriptor, tcpSocket);

    getDataFromUrl("https:/" + routeData, socketDescriptor);
}

VideoProxyServer::VideoProxyServer(QObject *parent)
    : QTcpServer{parent}
{
    connect(m_manager, &QNetworkAccessManager::finished, this, &VideoProxyServer::finishedResponse);
}

QByteArray VideoProxyServer::getRoute(QByteArray bytes)
{
    auto index = bytes.indexOf("\r\n");
    auto routeHeader = bytes.mid(0, index);

    auto parts = routeHeader.split(' ');
    return parts.count() >= 2 ? parts[1] : "";
}

void VideoProxyServer::closeSocket(QTcpSocket *socket)
{
    if (socket->state() != QAbstractSocket::UnconnectedState) socket->waitForBytesWritten(2000);
    socket->disconnectFromHost();
    delete socket;
}

QByteArray VideoProxyServer::readAllAvailableBytesFromSocket(QTcpSocket * socket)
{
    QByteArray result;

    while (true) {
        socket->waitForReadyRead(100);
        auto bytesCount = socket->bytesAvailable();
        if (bytesCount == 0 || socket->atEnd()) return result;

        result.append(socket->read(bytesCount));
    }

    return result;
}

void VideoProxyServer::getDataFromUrl(QString url, int socketDescriptor)
{
    auto urlAsUrl = QUrl(url);
    QNetworkRequest request(urlAsUrl);
    request.setRawHeader("User-Agent", "Anilibria CP Client");
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

    auto reply = m_manager->get(request);
    reply->setProperty("socket", socketDescriptor);
}

void VideoProxyServer::finishedResponse(QNetworkReply *reply)
{
    auto data = reply->readAll();

    auto contentType = reply->header(QNetworkRequest::ContentTypeHeader);

    auto isPlaylist = contentType.toString().toLower() == "application/x-mpegurl";

    if (isPlaylist) data = data.replace("https://", "http://localhost:" + QString::number(VideoProxyPort).toUtf8() + "/");

    auto socketDescriptor = reply->property("socket").toInt();

    QTcpSocket* tcpSocket = nullptr;

    if (!m_sockets.contains(socketDescriptor)) {
        tcpSocket = nullptr;
    } else {
        tcpSocket = m_sockets.value(socketDescriptor);
    }

    if (tcpSocket == nullptr) return;

    auto header = "HTTP/1.1 200 OK\r\nServer: nginx\r\nContent-Type: " + contentType.toString() + "\r\nContent-Length: " + QString::number(data.count()) + "\r\n\r\n";

    tcpSocket->write(header.toUtf8() + data);
    tcpSocket->waitForBytesWritten(10000);
    tcpSocket->disconnectFromHost();
    delete tcpSocket;

    m_sockets.remove(socketDescriptor);
}
