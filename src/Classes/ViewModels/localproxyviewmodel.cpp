#include "localproxyviewmodel.h"

LocalProxyViewModel::LocalProxyViewModel(QObject *parent)
    : QObject{parent}
{

}

void LocalProxyViewModel::setUsingProxy(bool usingProxy) noexcept
{
    if (IsUsingVideoProxy == usingProxy) return;

    if (usingProxy && m_server == nullptr) {
        m_server = new VideoProxyServer();
        if (m_server->listen(QHostAddress::Any, VideoProxyPort)) {
            VideoProxyPort = m_server->serverPort();
            qDebug() << "Video Proxy Started on port " << VideoProxyPort;
        } else {
            return;
        }
    }
    if (!usingProxy && m_server != nullptr) {
        m_server->close();
        VideoProxyPort = 0;
    }

    IsUsingVideoProxy = usingProxy;
    emit usingProxyChanged();
}
