/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2021 Roman Vladimirov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
