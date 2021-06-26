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

#include "apiserviceconfigurator.h"
#include <QFile>
#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonDocument>

ApiServiceConfigurator::ApiServiceConfigurator(QObject *parent) : QObject(parent)
{
    QFile settingsFile(getConfigurationPath());
    if (settingsFile.exists()) {
        if (settingsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            auto content = settingsFile.readAll();
            auto document = QJsonDocument::fromJson(content);
            auto rootObject = document.object();
            setConfiguration(rootObject.value("apiPath").toString(), rootObject.value("staticPath").toString());
            settingsFile.close();
        }
    } else {
        setConfiguration("", "");
    }
}

void ApiServiceConfigurator::saveApiConfiguration(const QString &apiAddress, const QString &staticAddress)
{
    setConfiguration(apiAddress, staticAddress);

    QFile settingsFile(getConfigurationPath());
    if (!settingsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        //TODO: show user error message
        return;
    }
    QJsonObject rootObject;

    rootObject["apiPath"] = m_ApiAddress;
    rootObject["staticPath"] = m_StaticAddress;

    QJsonDocument jsonDocument(rootObject);

    settingsFile.write(jsonDocument.toJson());

    settingsFile.close();
}

void ApiServiceConfigurator::restoreDefault()
{
    QFile settingsFile(getConfigurationPath());
    if (settingsFile.exists()) settingsFile.remove();

    setConfiguration("", "");
}

void ApiServiceConfigurator::setConfiguration(const QString &apiAddress, const QString &staticAddress)
{
    m_ApiAddress = apiAddress;
    emit apiAddressChanged();

    m_StaticAddress = staticAddress;
    emit staticAddressChanged();

    m_IsDefault = apiAddress.isEmpty() && staticAddress.isEmpty();
    emit isDefaultChanged();
}

QString ApiServiceConfigurator::getConfigurationPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/api.settings";
}
