#include "apiserviceconfigurator.h"
#include <QFile>
#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonDocument>

ApiServiceConfigurator::ApiServiceConfigurator(QObject *parent) : QObject(parent)
{
    QFile settingsFile(getConfiguratuionPath());
    if (settingsFile.exists()) {
        if (settingsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            auto content = settingsFile.readAll();
            auto document = QJsonDocument::fromJson(content);
            auto rootObject = document.object();
            setConfiguration(rootObject.value("apiPath").toString(), rootObject.value("staticPath").toString());
        }
    } else {
        setConfiguration("", "");
    }
}

void ApiServiceConfigurator::saveApiConfiguration(const QString &apiAddress, const QString &staticAddress)
{
    setConfiguration(apiAddress, staticAddress);

    QFile settingsFile(getConfiguratuionPath());
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
    QFile settingsFile(getConfiguratuionPath());
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

QString ApiServiceConfigurator::getConfiguratuionPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/api.settings";
}
