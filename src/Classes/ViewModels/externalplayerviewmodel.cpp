#include "externalplayerviewmodel.h"

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

}

void ExternalPlayerViewModel::stop() noexcept
{

}

void ExternalPlayerViewModel::play() noexcept
{

}

void ExternalPlayerViewModel::seek(int position) noexcept
{

}

void ExternalPlayerViewModel::open(const QString &source) noexcept
{

}
