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

private:
    bool m_playerButtonVisible;
    bool m_pauseButtonVisible;
    bool m_opened;
    bool m_isTopMost;
    int m_windowCursorShape;
    int m_panelOpacity;

public:
    explicit OnlinePlayerWindowViewModel(QObject *parent = nullptr);

    bool playerButtonVisible() const noexcept { return m_playerButtonVisible; };
    bool pauseButtonVisible() const noexcept { return m_pauseButtonVisible; };
    bool opened() const noexcept { return m_opened; }
    bool isTopMost() const noexcept { return m_isTopMost; }
    int windowCursorShape() const noexcept { return m_windowCursorShape; }
    int panelOpacity() const noexcept { return m_panelOpacity; }

    void setPlayerButtonVisible(const bool& playerButtonVisible) noexcept;
    void setPauseButtonVisible(const bool& pauseButtonVisible) noexcept;
    void setOpened(const bool& opened) noexcept;
    void setIsTopMost(const bool& isTopMost) noexcept;
    void setWindowCursorShape(const int& windowCursorShape) noexcept;
    void setPanelOpacity(int panelOpacity) noexcept;

    Q_INVOKABLE void playbackStateChanged(const bool& isPlaying);
    Q_INVOKABLE void hideControlPanel();
    Q_INVOKABLE void showPanel();

signals:
    void playerButtonVisibleChanged();
    void pauseButtonVisibleChanged();
    void openedChanged();
    void isTopMostChanged();
    void controlPanelOpacityChanged();
    void windowCursorShapeChanged();
    void panelOpacityChanged();

};

#endif // ONLINEPLAYERWINDOWVIEWMODEL_H
