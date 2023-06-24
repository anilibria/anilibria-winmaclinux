#include "rutubeexternalplayer.h"

RutubeExternalPlayer::RutubeExternalPlayer(QObject *parent)
    : ExternalPlayerBase{parent}
{

}

void RutubeExternalPlayer::trySetNewState(const QString &state)
{
    emit stateChanged(state);
}

void RutubeExternalPlayer::trySetNewVolume(int volume)
{
    emit volumeChanged(volume);
}

void RutubeExternalPlayer::trySetSource(const QString &path)
{
    emit sourceChanged(path);
}
