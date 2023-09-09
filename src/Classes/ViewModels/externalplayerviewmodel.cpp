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

void ExternalPlayerViewModel::setTorrentStreamHost(QString torrentStreamHost) noexcept
{
    if (m_torrentStreamHost == torrentStreamHost) return;

    m_torrentStreamHost = torrentStreamHost;
    emit torrentStreamHostChanged();
}

void ExternalPlayerViewModel::setTorrentStreamPort(int torrentStreamPort) noexcept
{
    if (m_torrentStreamPort == torrentStreamPort) return;

    m_torrentStreamPort = torrentStreamPort;
    emit torrentStreamPortChanged();
}

void ExternalPlayerViewModel::setTorrentStreamActive(bool torrentStreamActive) noexcept
{
    if (m_torrentStreamActive == torrentStreamActive) return;

    m_torrentStreamActive = torrentStreamActive;
    emit torrentStreamActiveChanged();
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
    if (m_player == nullptr) return;

    m_player->trySetNewState(pausedState);
}

void ExternalPlayerViewModel::stop() noexcept
{
    if (m_player == nullptr) return;

    m_player->trySetNewState(stoppedState);
}

void ExternalPlayerViewModel::play() noexcept
{
    if (m_player == nullptr) return;

    m_player->trySetNewState(playingState);
}

void ExternalPlayerViewModel::seek(int position) noexcept
{
    if (m_player == nullptr) return;

    m_player->trySetSeek(position);
}

void ExternalPlayerViewModel::open(const QString &source) noexcept
{
    if (m_player == nullptr) return;

    m_player->trySetSource(source);
}

void ExternalPlayerViewModel::changeVolume(int value) noexcept
{
    m_player->trySetNewVolume(value);
}

void ExternalPlayerViewModel::nextVideo() noexcept
{

}

void ExternalPlayerViewModel::previousVideo() noexcept
{

}

void ExternalPlayerViewModel::addWebSocketPlayer() noexcept
{
    if (!m_torrentStreamActive) return;

    auto player = new WebSocketExternalPlayer(this, m_torrentStreamHost, m_torrentStreamPort, "rt");
    m_player = player;
}
