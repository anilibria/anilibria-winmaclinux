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

#ifndef APISERVICECONFIGURATOR_H
#define APISERVICECONFIGURATOR_H

#include <QObject>

class ApiServiceConfigurator : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isDefault READ isDefault NOTIFY isDefaultChanged)
    Q_PROPERTY(QString apiAddress READ apiAddress NOTIFY apiAddressChanged)
    Q_PROPERTY(QString staticAddress READ staticAddress NOTIFY staticAddressChanged)
private:
    bool m_IsDefault;
    QString m_ApiAddress;
    QString m_StaticAddress;

public:
    explicit ApiServiceConfigurator(QObject *parent = nullptr);

    bool isDefault() const { return m_IsDefault; }
    QString apiAddress() const { return m_ApiAddress; }
    QString staticAddress() const { return m_StaticAddress; }

    Q_INVOKABLE void saveApiConfiguration(const QString& apiAddress, const QString& staticAddress);
    Q_INVOKABLE void restoreDefault();

private:
    void setConfiguration(const QString& apiAddress, const QString& staticAddress);
    QString getConfigurationPath();

signals:
    void isDefaultChanged();
    void apiAddressChanged();
    void staticAddressChanged();

};

#endif // APISERVICECONFIGURATOR_H
