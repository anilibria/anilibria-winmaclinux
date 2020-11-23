#include "proxyconfigurator.h"
#include <QFile>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkProxy>

ProxyConfigurator::ProxyConfigurator(QObject *parent) : QObject(parent), m_ProxyType(""), m_Address(""), m_Port(0), m_UserName(""), m_Password("")
{
    QFile settingsFile(getConfigurationPath());
    if (!settingsFile.exists()) return;

    if (settingsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        auto content = settingsFile.readAll();
        auto document = QJsonDocument::fromJson(content);
        auto rootObject = document.object();

        m_ProxyType = rootObject.value("type").toString();
        m_Address = rootObject.value("address").toString();
        m_Port = rootObject.value("port").toInt();
        m_UserName = rootObject.value("userName").toString();
        m_Password = rootObject.value("password").toString();

        settingsFile.close();

        applyProxy();
    }
}

void ProxyConfigurator::setProxyType(const QString &proxyType) noexcept
{
    if (m_ProxyType == proxyType) return;

    m_ProxyType = proxyType;
    emit proxyTypeChanged();
}

void ProxyConfigurator::setAddress(const QString &address) noexcept
{
    if (m_Address == address) return;

    m_Address = address;
    emit addressChanged();
}

void ProxyConfigurator::setPort(const quint64 &port) noexcept
{
    if (m_Port == port) return;

    m_Port = port;
    emit portChanged();
}

void ProxyConfigurator::setUserName(const QString &userName) noexcept
{
    if (m_UserName == userName) return;

    m_UserName = userName;
    emit userNameChanged();
}

void ProxyConfigurator::setPassword(const QString &password) noexcept
{
    if (m_Password == password) return;

    m_Password = password;
    emit passwordChanged();
}

void ProxyConfigurator::saveProxy()
{
    QFile settingsFile(getConfigurationPath());
    if (!settingsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        //TODO: show user error message
        return;
    }
    QJsonObject rootObject;

    rootObject["type"] = m_ProxyType;
    rootObject["address"] = m_Address;
    rootObject["port"] = QJsonValue((qint64)m_Port);
    rootObject["userName"] = m_UserName;
    rootObject["password"] = m_Password;

    QJsonDocument jsonDocument(rootObject);

    settingsFile.write(jsonDocument.toJson());

    settingsFile.close();

    applyProxy();
}

void ProxyConfigurator::disableProxy()
{
    setProxyType("");
    setAddress("");
    setPort(0);
    setUserName("");
    setPassword("");

    QFile settingsFile(getConfigurationPath());
    settingsFile.remove();

    QNetworkProxy::setApplicationProxy(QNetworkProxy::NoProxy);
}

QString ProxyConfigurator::getConfigurationPath() const noexcept
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/proxy.settings";
}

void ProxyConfigurator::applyProxy() const
{
    QNetworkProxy proxy;
    proxy.setType(m_ProxyType == "SOCKS5" ? QNetworkProxy::Socks5Proxy : QNetworkProxy::HttpProxy);
    proxy.setHostName(m_Address);
    proxy.setPort(m_Port);
    proxy.setUser(m_Address);
    proxy.setPassword(m_Password);
    QNetworkProxy::setApplicationProxy(proxy);
}
