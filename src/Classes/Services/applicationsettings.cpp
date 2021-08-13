/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2020 Roman Vladimirov

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

#include "applicationsettings.h"
#include <QCoreApplication>

ApplicationSettings::ApplicationSettings(QObject *parent) : QObject(parent)
{
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

int ApplicationSettings::windowWidth()
{
    return m_Settings->value("windowwidth", 0).toInt();
}

void ApplicationSettings::setWindowWidth(int windowWidth)
{
    m_Settings->setValue("windowwidth", windowWidth);

    emit windowWidthChanged(windowWidth);
}

int ApplicationSettings::windowHeight()
{
    return m_Settings->value("windowheight", 0).toInt();
}

void ApplicationSettings::setWindowHeight(int windowHeight)
{
    m_Settings->setValue("windowheight", windowHeight);

    emit windowHeightChanged(windowHeight);
}

int ApplicationSettings::windowX()
{
    return m_Settings->value("windowx", 0).toInt();
}

void ApplicationSettings::setWindowX(int windowX)
{
    m_Settings->setValue("windowx", windowX);

    emit windowXChanged(windowX);
}

int ApplicationSettings::windowY()
{
    return m_Settings->value("windowy", 0).toInt();
}

void ApplicationSettings::setWindowY(int windowY)
{
    m_Settings->setValue("windowy", windowY);

    emit windowYChanged(windowY);
}

bool ApplicationSettings::isDarkTheme()
{
    return m_Settings->value("darktheme", false).toBool();
}

void ApplicationSettings::setIsDarkTheme(bool isDarkTheme)
{
    m_Settings->setValue("darktheme", isDarkTheme);

    emit isDarkThemeChanged(isDarkTheme);
}

int ApplicationSettings::remotePort()
{
    return m_Settings->value("remoteport", 12345).toInt();
}

void ApplicationSettings::setRemotePort(int remotePort)
{
    m_Settings->setValue("remoteport", remotePort);

    emit remotePortChanged(remotePort);
}

bool ApplicationSettings::sendVolumeToRemote()
{
    return m_Settings->value("sendvolumetoremote", true).toBool();
}

void ApplicationSettings::setSendVolumeToRemote(bool sendVolumeToRemote)
{
    m_Settings->setValue("sendvolumetoremote", sendVolumeToRemote);

    emit sendVolumeToRemoteChanged(sendVolumeToRemote);
}

bool ApplicationSettings::sendPlaybackToRemote()
{
    return m_Settings->value("sendplaybacktoremote", true).toBool();
}

void ApplicationSettings::setSendPlaybackToRemote(bool sendPlaybackToRemote)
{
    m_Settings->setValue("sendplaybacktoremote", sendPlaybackToRemote);

    emit sendPlaybackToRemoteChanged(sendPlaybackToRemote);
}

bool ApplicationSettings::useCustomToolbar()
{
    return m_Settings->value("usecustomtoolbar", true).toBool();
}

void ApplicationSettings::setUseCustomToolbar(bool useCustomToolbar)
{
    m_Settings->setValue("usecustomtoolbar", useCustomToolbar);

    emit useCustomToolbarChanged(useCustomToolbar);
}
