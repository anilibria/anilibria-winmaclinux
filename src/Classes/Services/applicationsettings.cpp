#include "applicationsettings.h"
#include <QCoreApplication>

ApplicationSettings::ApplicationSettings(QObject *parent) : QObject(parent)
{
    QCoreApplication::setOrganizationDomain("anilibria.tv");
    QCoreApplication::setOrganizationName("EmptyFlow");
    QCoreApplication::setApplicationName("AnilibriaDesktopClient");

    m_Settings = new QSettings;
}

QString ApplicationSettings::userToken()
{
    return m_Settings->value("usertoken", "").toString();
}

void ApplicationSettings::setUserToken(QString &token)
{
    m_Settings->setValue("usertoken", token);

    emit userTokenChanged(token);
}
