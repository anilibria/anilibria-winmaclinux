#include "externalplayerviewmodel.h"
#include "../RemotePlayer/websocketexternalplayer.h"

ExternalPlayerViewModel::ExternalPlayerViewModel(QObject *parent)
    : QObject{parent}
{

}

void ExternalPlayerViewModel::setVolume(int volume) noexcept
{
    if (m_volume == volume) return;
    if (m_volume < 0) return;
    if (m_volume > 100) return;

    m_volume = volume;
    emit volumeChanged();
}

void ExternalPlayerViewModel::setState(const QString &state) noexcept
{
    if (state != stoppedState && m_state != pausedState && m_state != playingState) return;

    m_state = state;
    emit isPlayingChanged();
    emit isPausedChanged();
    emit isStoppedChanged();
}

void ExternalPlayerViewModel::pause() noexcept
{
    foreach (auto player, m_players) {
        player->trySetNewState(pausedState);
    }
}

void ExternalPlayerViewModel::stop() noexcept
{
    foreach (auto player, m_players) {
        player->trySetNewState(stoppedState);
    }
}

void ExternalPlayerViewModel::play() noexcept
{
    foreach (auto player, m_players) {
        player->trySetNewState(playingState);
    }
}

void ExternalPlayerViewModel::seek(int position) noexcept
{
    foreach (auto player, m_players) {
        player->trySetSeek(position);
    }
}

void ExternalPlayerViewModel::open(const QString &source) noexcept
{
    foreach (auto player, m_players) {
        player->trySetSource(source);
    }
}

void ExternalPlayerViewModel::addWebSocketPlayer() noexcept
{
    auto player = new WebSocketExternalPlayer(this);
    m_players.append(player);
}
