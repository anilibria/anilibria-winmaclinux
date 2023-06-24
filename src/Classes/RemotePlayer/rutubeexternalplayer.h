#ifndef RUTUBEEXTERNALPLAYER_H
#define RUTUBEEXTERNALPLAYER_H

#include <QObject>
#include "externalplayerbase.h"

class RutubeExternalPlayer : public ExternalPlayerBase
{
    Q_OBJECT
public:
    explicit RutubeExternalPlayer(QObject *parent = nullptr);

    void trySetNewState(const QString& state) override;
    void trySetNewVolume(int volume) override;
    void trySetSource(const QString& path) override;

signals:

};

#endif // RUTUBEEXTERNALPLAYER_H
