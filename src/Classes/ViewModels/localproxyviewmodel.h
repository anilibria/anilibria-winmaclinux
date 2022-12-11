#ifndef LOCALPROXYVIEWMODEL_H
#define LOCALPROXYVIEWMODEL_H

#include <QObject>
#include <QTcpServer>
#include "../../globalconstants.h"
#include "../Servers/videoproxyserver.h"

class LocalProxyViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool usingProxy READ usingProxy WRITE setUsingProxy NOTIFY usingProxyChanged)

private:
    VideoProxyServer* m_server { nullptr };

public:
    explicit LocalProxyViewModel(QObject *parent = nullptr);

    bool usingProxy() const noexcept { return IsUsingVideoProxy; }
    void setUsingProxy(bool usingProxy) noexcept;

signals:
    void usingProxyChanged();
    void proxyIsAvailableChanged();

};

#endif // LOCALPROXYVIEWMODEL_H
