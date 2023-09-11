#ifndef EXTERNALPLAYERBASE_H
#define EXTERNALPLAYERBASE_H

#include <QObject>

class ExternalPlayerBase : public QObject
{
    Q_OBJECT
public:
    explicit ExternalPlayerBase(QObject *parent = nullptr);

    virtual void trySetNewState(const QString& state) = 0;
    virtual void trySetNewVolume(int volume) = 0;
    virtual void trySetSource(const QString& path) = 0;
    virtual void trySetSeek(int position) = 0;
    virtual void tryMuted(bool mute) = 0;
    virtual void closePlayer() = 0;

signals:
    void stateChanged(const QString& state);
    void volumeChanged(int volume);
    void sourceChanged(const QString& path);
    void positionChanged(int position);
    void mutedChanged(bool muted);

};

#endif // EXTERNALPLAYERBASE_H
