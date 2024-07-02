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

QString ExternalPlayerViewModel::position() const noexcept
{
    return getDisplayTimeFromSeconds(m_position);
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

    auto videos = m_releases->getReleaseVideos(releaseId);

    foreach (auto item, videos) {
        if (item->rutubeId().isEmpty()) continue;

        m_seriesMap.insert(item->order(), item->rutubeId());
    }

    auto keys = m_seriesMap.keys();
    std::sort(
        keys.begin(),
        keys.end(),
        [](int left, int right) {
            return left < right;
        }
    );

    foreach (auto key, keys) {
        m_series.append(m_seriesMap[key]);
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

QString ExternalPlayerViewModel::getZeroBasedDigit(int digit) const
{
    if (digit < 10) return "0" + QString::number(digit);
    return QString::number(digit);
}

QString ExternalPlayerViewModel::getDisplayTimeFromSeconds(int seconds) const
{
    int days = floor(seconds / (3600 * 24));
    seconds -= days * 3600 * 24;
    int hours = floor(seconds / 3600);
    seconds -= hours * 3600;
    int minutes = floor(seconds / 60);
    seconds  -= minutes * 60;

    QString result;
    result.append(getZeroBasedDigit(hours));
    result.append(":");
    result.append(getZeroBasedDigit(minutes));
    result.append(":");
    result.append(getZeroBasedDigit(seconds));
    return result;
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
