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

#include "onlineplayerwindowviewmodel.h"

OnlinePlayerWindowViewModel::OnlinePlayerWindowViewModel(QObject *parent) : QObject(parent),
    m_playerButtonVisible(false),
    m_pauseButtonVisible(false),
    m_opened(true),
    m_isTopMost(false),
    m_windowCursorShape(Qt::ArrowCursor),
    m_panelOpacity(1),
    m_isStandartPlayer(true)
{
#ifdef USE_VLC_PLAYER
    m_isStandartPlayer = false;
#endif
}

void OnlinePlayerWindowViewModel::setPlayerButtonVisible(const bool &playerButtonVisible) noexcept
{
    if (playerButtonVisible == m_playerButtonVisible) return;

    m_playerButtonVisible = playerButtonVisible;
    emit playerButtonVisibleChanged();
}

void OnlinePlayerWindowViewModel::setPauseButtonVisible(const bool &pauseButtonVisible) noexcept
{
    if (pauseButtonVisible == m_pauseButtonVisible) return;

    m_pauseButtonVisible = pauseButtonVisible;
    emit pauseButtonVisibleChanged();
}

void OnlinePlayerWindowViewModel::setOpened(const bool &opened) noexcept
{
    if (opened == m_opened) return;

    m_opened = opened;
    emit openedChanged();
}

void OnlinePlayerWindowViewModel::setIsTopMost(const bool &isTopMost) noexcept
{
    if (isTopMost == m_isTopMost) return;

    m_isTopMost = isTopMost;
    emit isTopMostChanged();
}

void OnlinePlayerWindowViewModel::setWindowCursorShape(const int &windowCursorShape) noexcept
{
    if (windowCursorShape == m_windowCursorShape) return;

    m_windowCursorShape = windowCursorShape;
    emit windowCursorShapeChanged();
}

void OnlinePlayerWindowViewModel::setPanelOpacity(int panelOpacity) noexcept
{
    if (m_panelOpacity != panelOpacity) {
        m_panelOpacity = panelOpacity;
        emit panelOpacityChanged();
    }
}

void OnlinePlayerWindowViewModel::setIsStandartPlayer(bool isStandartPlayer) noexcept
{
    if (isStandartPlayer == m_isStandartPlayer) return;

    m_isStandartPlayer = isStandartPlayer;
    emit isStandartPlayerChanged();
}

void OnlinePlayerWindowViewModel::playbackStateChanged(const bool &isPlaying)
{
    setPlayerButtonVisible(!isPlaying);
    setPauseButtonVisible(!m_playerButtonVisible);
}

void OnlinePlayerWindowViewModel::hideControlPanel()
{
    setPanelOpacity(0);
    setWindowCursorShape(Qt::BlankCursor);
}

void OnlinePlayerWindowViewModel::showPanel()
{
    setPanelOpacity(1);
    setWindowCursorShape(Qt::ArrowCursor);
}
