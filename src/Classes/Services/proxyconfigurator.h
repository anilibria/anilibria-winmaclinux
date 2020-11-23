#ifndef PROXYCONFIGURATOR_H
#define PROXYCONFIGURATOR_H

#include <QObject>

class ProxyConfigurator : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString proxyType READ proxyType WRITE setProxyType NOTIFY proxyTypeChanged)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(quint64 port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)

private:
    QString m_ProxyType;
    QString m_Address;
    quint64 m_Port;
    QString m_UserName;
    QString m_Password;

public:
    explicit ProxyConfigurator(QObject *parent = nullptr);

    QString proxyType() const { return m_ProxyType; }
    QString address() const { return m_Address; }
    quint64 port() const { return m_Port; }
    QString userName() const { return m_UserName; }
    QString password() const { return m_Password; }

    void setProxyType(const QString& proxyType) noexcept;
    void setAddress(const QString& address) noexcept;
    void setPort(const quint64& port) noexcept;
    void setUserName(const QString& userName) noexcept;
    void setPassword(const QString& password) noexcept;

    Q_INVOKABLE void saveProxy();
    Q_INVOKABLE void disableProxy();

private:
    QString getConfigurationPath() const noexcept;
    void applyProxy() const;

signals:
    void proxyTypeChanged();
    void addressChanged();
    void portChanged();
    void userNameChanged();
    void passwordChanged();

};

#endif // PROXYCONFIGURATOR_H
