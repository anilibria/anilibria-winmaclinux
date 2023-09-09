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
    Q_PROPERTY(bool torrentStreamActive READ torrentStreamActive WRITE setTorrentStreamActive NOTIFY torrentStreamActiveChanged)
    Q_PROPERTY(QString torrentStreamHost READ torrentStreamHost WRITE setTorrentStreamHost NOTIFY torrentStreamHostChanged)
    Q_PROPERTY(int torrentStreamPort READ torrentStreamPort WRITE setTorrentStreamPort NOTIFY torrentStreamPortChanged)

private:
    const QString stoppedState { "stop" };
    const QString pausedState { "pause" };
    const QString playingState { "play" };
    int m_volume { 50 };
    QString m_state { stoppedState };
    QString m_torrentStreamHost { "" };
    int m_torrentStreamPort { 0 };
    bool m_torrentStreamActive { false };
    ExternalPlayerBase* m_player { nullptr };

public:
    explicit ExternalPlayerViewModel(QObject *parent = nullptr);

    int volume() const noexcept { return m_volume; }
    void setVolume(int volume) noexcept;

    QString torrentStreamHost() const noexcept { return m_torrentStreamHost; }
    void setTorrentStreamHost(QString torrentStreamHost) noexcept;

    int torrentStreamPort() const noexcept { return m_torrentStreamPort; }
    void setTorrentStreamPort(int torrentStreamPort) noexcept;

    bool torrentStreamActive() const noexcept { return m_torrentStreamActive; }
    void setTorrentStreamActive(bool torrentStreamActive) noexcept;

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
    Q_INVOKABLE void changeVolume(int value) noexcept;
    Q_INVOKABLE void nextVideo() noexcept;
    Q_INVOKABLE void previousVideo() noexcept;
    Q_INVOKABLE void addWebSocketPlayer() noexcept;

signals:
    void volumeChanged();
    void isPlayingChanged();
    void isPausedChanged();
    void isStoppedChanged();
    void openedChanged();
    void torrentStreamHostChanged();
    void torrentStreamPortChanged();
    void torrentStreamActiveChanged();

};

#endif // EXTERNALPLAYERVIEWMODEL_H
