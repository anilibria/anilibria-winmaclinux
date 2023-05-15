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

#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QObject>
#include <QSettings>

class ApplicationSettings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString userToken READ userToken WRITE setUserToken NOTIFY userTokenChanged)
    Q_PROPERTY(int windowWidth READ windowWidth WRITE setWindowWidth NOTIFY windowWidthChanged)
    Q_PROPERTY(int windowHeight READ windowHeight WRITE setWindowHeight NOTIFY windowHeightChanged)
    Q_PROPERTY(int windowX READ windowX WRITE setWindowX NOTIFY windowXChanged)
    Q_PROPERTY(int windowY READ windowY WRITE setWindowY NOTIFY windowYChanged)
    Q_PROPERTY(int remotePort READ remotePort WRITE setRemotePort NOTIFY remotePortChanged)
    Q_PROPERTY(bool sendVolumeToRemote READ sendVolumeToRemote WRITE setSendVolumeToRemote NOTIFY sendVolumeToRemoteChanged)
    Q_PROPERTY(bool sendPlaybackToRemote READ sendPlaybackToRemote WRITE setSendPlaybackToRemote NOTIFY sendPlaybackToRemoteChanged);
    Q_PROPERTY(bool isMaximize READ isMaximize WRITE setIsMaximize NOTIFY isMaximizeChanged)
    Q_PROPERTY(int normalX READ normalX WRITE setNormalX NOTIFY normalXChanged)
    Q_PROPERTY(int normalY READ normalY WRITE setNormalY NOTIFY normalYChanged)
    Q_PROPERTY(int normalWidth READ normalWidth WRITE setNormalWidth NOTIFY normalWidthChanged)
    Q_PROPERTY(int normalHeight READ normalHeight WRITE setNormalHeight NOTIFY normalHeightChanged)

private:
    QSettings* m_Settings;

public:
    explicit ApplicationSettings(QObject *parent = nullptr);

    QString userToken();
    void setUserToken(QString& token);

    int windowWidth();
    void setWindowWidth(int windowWidth);

    int windowHeight();
    void setWindowHeight(int windowHeight);

    int windowX();
    void setWindowX(int windowX);

    int windowY();
    void setWindowY(int windowY);

    int remotePort();
    void setRemotePort(int remotePort);

    bool sendVolumeToRemote();
    void setSendVolumeToRemote(bool sendVolumeToRemote);

    bool sendPlaybackToRemote();
    void setSendPlaybackToRemote(bool sendPlaybackToRemote);

    bool isMaximize() noexcept;
    void setIsMaximize(bool isMaximize) noexcept;

    int normalX() noexcept;
    void setNormalX(int normalX) noexcept;

    int normalY() noexcept;
    void setNormalY(int normalY) noexcept;

    int normalWidth() noexcept;
    void setNormalWidth(int normalWidth) noexcept;

    int normalHeight() noexcept;
    void setNormalHeight(int normalHeight) noexcept;


signals:
    void userTokenChanged(QString& token);
    void windowHeightChanged(int windowHeight);
    void windowWidthChanged(int windowWidth);
    void windowXChanged(int windowX);
    void windowYChanged(int windowY);
    void remotePortChanged(int remotePort);
    void sendVolumeToRemoteChanged(bool sendVolumeToRemote);
    void sendPlaybackToRemoteChanged(bool sendPlaybackToRemote);
    void isMaximizeChanged();
    void normalXChanged();
    void normalYChanged();
    void normalWidthChanged();
    void normalHeightChanged();

};

#endif // APPLICATIONSETTINGS_H
