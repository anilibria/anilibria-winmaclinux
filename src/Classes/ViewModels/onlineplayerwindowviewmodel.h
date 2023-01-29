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

#ifndef ONLINEPLAYERWINDOWVIEWMODEL_H
#define ONLINEPLAYERWINDOWVIEWMODEL_H

#include <QObject>
#include <QDebug>

class OnlinePlayerWindowViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool playerButtonVisible READ playerButtonVisible WRITE setPlayerButtonVisible NOTIFY playerButtonVisibleChanged)
    Q_PROPERTY(bool pauseButtonVisible READ pauseButtonVisible WRITE setPauseButtonVisible NOTIFY pauseButtonVisibleChanged)
    Q_PROPERTY(bool opened READ opened WRITE setOpened NOTIFY openedChanged)
    Q_PROPERTY(bool isTopMost READ isTopMost WRITE setIsTopMost NOTIFY isTopMostChanged)
    Q_PROPERTY(int windowCursorShape READ windowCursorShape WRITE setWindowCursorShape NOTIFY windowCursorShapeChanged)
    Q_PROPERTY(int panelOpacity READ panelOpacity WRITE setPanelOpacity NOTIFY panelOpacityChanged)
    Q_PROPERTY(QString playerComponent READ playerComponent NOTIFY playerComponentChanged)
    Q_PROPERTY(QString playerOutputComponent READ playerOutputComponent NOTIFY playerOutputComponentChanged)
    Q_PROPERTY(bool supportOutput READ supportOutput NOTIFY supportOutputChanged)
    Q_PROPERTY(QStringList players READ players NOTIFY playersChanged)
    Q_PROPERTY(QString selectedPlayer READ selectedPlayer NOTIFY selectedPlayerChanged)
    Q_PROPERTY(bool isHasVlc READ isHasVlc NOTIFY isHasVlcChanged)
    Q_PROPERTY(bool isSelectedQtAv READ isSelectedQtAv NOTIFY isSelectedQtAvChanged)

private:
    bool m_playerButtonVisible;
    bool m_pauseButtonVisible;
    bool m_opened;
    bool m_isTopMost;
    int m_windowCursorShape;
    int m_panelOpacity;
    QString m_playerComponent { "" };
    QString m_playerOutputComponent { "" };
    bool m_supportOutput { false };
    QStringList m_players { QStringList() };
    QString m_selectedPlayer { "" };
    QMap<QString, QString> m_playerComponents { QMap<QString, QString>() };
    QMap<QString, QString> m_playerOutputComponents { QMap<QString, QString>() };
    bool m_isHasVlc { false };
    bool m_isSelectedQtAv { false };
    const QString nameVLCPlayer { "VLC" };
    const QString nameQtAvPlayer { "QtAv" };

public:
    explicit OnlinePlayerWindowViewModel(QObject *parent = nullptr);

    bool playerButtonVisible() const noexcept { return m_playerButtonVisible; };
    bool pauseButtonVisible() const noexcept { return m_pauseButtonVisible; };
    bool opened() const noexcept { return m_opened; }
    bool isTopMost() const noexcept { return m_isTopMost; }
    int windowCursorShape() const noexcept { return m_windowCursorShape; }
    int panelOpacity() const noexcept { return m_panelOpacity; }
    QString playerComponent() const noexcept { return m_playerComponent; };
    QString playerOutputComponent() const noexcept { return m_playerOutputComponent; }
    bool supportOutput() const noexcept { return m_supportOutput; }

    void setPlayerButtonVisible(const bool& playerButtonVisible) noexcept;
    void setPauseButtonVisible(const bool& pauseButtonVisible) noexcept;
    void setOpened(const bool& opened) noexcept;
    void setIsTopMost(const bool& isTopMost) noexcept;
    void setWindowCursorShape(const int& windowCursorShape) noexcept;
    void setPanelOpacity(int panelOpacity) noexcept;

    QStringList players() const noexcept { return m_players; }

    QString selectedPlayer() const noexcept { return m_selectedPlayer; }

    bool isHasVlc() const noexcept { return m_isHasVlc; }
    bool isSelectedQtAv() const noexcept { return m_isSelectedQtAv; }

    Q_INVOKABLE void playbackStateChanged(const bool& isPlaying);
    Q_INVOKABLE void hideControlPanel();
    Q_INVOKABLE void showPanel();
    Q_INVOKABLE void clearCurrentPlayer();
    Q_INVOKABLE void changePlayer(const QString& player);

private:
    void fillSupportedPlayers();

signals:
    void playerButtonVisibleChanged();
    void pauseButtonVisibleChanged();
    void openedChanged();
    void isTopMostChanged();
    void controlPanelOpacityChanged();
    void windowCursorShapeChanged();
    void panelOpacityChanged();
    void playerComponentChanged();
    void playerOutputComponentChanged();
    void supportOutputChanged();
    void playersChanged();
    void selectedPlayerChanged();
    void isHasVlcChanged();
    void isSelectedQtAvChanged();

};

#endif // ONLINEPLAYERWINDOWVIEWMODEL_H
