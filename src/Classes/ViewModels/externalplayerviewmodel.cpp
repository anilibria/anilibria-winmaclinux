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

void ExternalPlayerViewModel::setReleasesViewModel(const ReleasesViewModel *viewmodel) noexcept
{
    if (m_releases != nullptr) return;

    m_releases = const_cast<ReleasesViewModel *>(viewmodel);
    emit releasesViewModelChanged();
}

void ExternalPlayerViewModel::setState(const QString &state) noexcept
{
    if (state != stoppedState && state != pausedState && state != playingState) return;

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

void ExternalPlayerViewModel::changeMute(bool muted) noexcept
{
    m_player->tryMuted(muted);
}

void ExternalPlayerViewModel::nextVideo() noexcept
{
    if (m_currentSeria >= m_series.count() - 1) return;

    m_currentSeria++;
    open(m_series[m_currentSeria]);
    emit currentSeriaChanged();
}

void ExternalPlayerViewModel::previousVideo() noexcept
{
    if (m_currentSeria == 0) return;

    m_currentSeria--;
    open(m_series[m_currentSeria]);
    emit currentSeriaChanged();
}

void ExternalPlayerViewModel::setWebSocketPlayer(int releaseId) noexcept
{
    if (!m_torrentStreamActive) return;

    m_series.clear();

    auto release = m_releases->getReleaseById(releaseId);
    if (release == nullptr) return;

    m_releaseName = release->title();
    emit releaseNameChanged();

    auto document = QJsonDocument::fromJson(release->videos().toUtf8());
    auto videosArray = document.array();

    foreach (auto item, videosArray) {
        if (!item.isObject()) continue;
        auto object = item.toObject();
        if (!object.contains("rutube_id")) continue;

        auto id = object["rutube_id"];
        if (!id.isString()) continue;

        m_series.append(id.toString());
    }

    auto player = new WebSocketExternalPlayer(this, m_torrentStreamHost, m_torrentStreamPort, "rt");
    player->setInitialVideo(m_series.first());
    connect(player, &WebSocketExternalPlayer::stateChanged, this, &ExternalPlayerViewModel::stateSynchronizedChanged);
    connect(player, &WebSocketExternalPlayer::volumeChanged, this, &ExternalPlayerViewModel::volumeSynchronizedChanged);
    connect(player, &WebSocketExternalPlayer::positionChanged, this, &ExternalPlayerViewModel::positionSynchronizedChanged);
    connect(player, &WebSocketExternalPlayer::playerConnected, this, &ExternalPlayerViewModel::playerConnected);
    connect(player, &WebSocketExternalPlayer::mutedChanged, this, &ExternalPlayerViewModel::mutedPlayerChanged);

    m_player = player;
    m_status = "Ожидание плеера";
    emit statusChanged();
}

void ExternalPlayerViewModel::closePlayer() noexcept
{
    if (m_player == nullptr) return;

    m_player->closePlayer();
    m_player = nullptr;
}

void ExternalPlayerViewModel::stateSynchronizedChanged(const QString &newState) noexcept
{
    setState(newState);
    if (m_status == "Ожидание плеера") {
        m_status = "";
        emit statusChanged();
    }
}

void ExternalPlayerViewModel::volumeSynchronizedChanged(int volume) noexcept
{
    setVolume(volume);
}

void ExternalPlayerViewModel::positionSynchronizedChanged(int position) noexcept
{
    m_position = position;
    emit positionChanged();
}

void ExternalPlayerViewModel::mutedPlayerChanged(bool muted) noexcept
{
    m_muted = muted;
    emit mutedChanged();
}

void ExternalPlayerViewModel::playerConnected() noexcept
{
    changeVolume(m_volume);
}
