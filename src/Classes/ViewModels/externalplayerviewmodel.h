#ifndef EXTERNALPLAYERVIEWMODEL_H
#define EXTERNALPLAYERVIEWMODEL_H

#include <QObject>
#include "../RemotePlayer/externalplayerbase.h"

class ExternalPlayerViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)
    Q_PROPERTY(bool isPaused READ isPaused NOTIFY isPausedChanged)
    Q_PROPERTY(bool isStopped READ isStopped NOTIFY isStoppedChanged)
    Q_PROPERTY(bool opened READ opened NOTIFY openedChanged)

private:
    const QString stoppedState { "stopped" };
    const QString pausedState { "paused" };
    const QString playingState { "playing" };
    int m_volume { 50 };
    QString m_state { stoppedState };
    QList<ExternalPlayerBase*> m_players { QList<ExternalPlayerBase*>() };

public:
    explicit ExternalPlayerViewModel(QObject *parent = nullptr);

    int volume() const noexcept { return m_volume; }
    void setVolume(int volume) noexcept;

    bool isPlaying() const noexcept { return false; }
    bool isPaused() const noexcept { return false; }
    bool isStopped() const noexcept { return false; }
    bool opened() const noexcept { return false; }

    void setState(const QString& state) noexcept;

    Q_INVOKABLE void pause() noexcept;
    Q_INVOKABLE void stop() noexcept;
    Q_INVOKABLE void play() noexcept;
    Q_INVOKABLE void seek(int position) noexcept;
    Q_INVOKABLE void open(const QString& source) noexcept;

signals:
    void volumeChanged();
    void isPlayingChanged();
    void isPausedChanged();
    void isStoppedChanged();
    void openedChanged();

};

#endif // EXTERNALPLAYERVIEWMODEL_H
