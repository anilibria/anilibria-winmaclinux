#ifndef WEBSOCKETEXTERNALPLAYER_H
#define WEBSOCKETEXTERNALPLAYER_H

#include <QObject>
#include <QWebSocket>
#include "externalplayerbase.h"

class WebSocketExternalPlayer : public ExternalPlayerBase
{
    Q_OBJECT
private:
    QWebSocket* m_socket { nullptr };

public:
    explicit WebSocketExternalPlayer(QObject *parent = nullptr);

    void trySetNewState(const QString& state) override;
    void trySetNewVolume(int volume) override;
    void trySetSource(const QString& path) override;
    void trySetSeek(int position) override;
    void tryMuted(bool mute) override;

private:
    void sendCommand(const QString& command, const QString& parameter);

private slots:
    void onTextMessageReceived(const QString &message);

signals:

};

#endif // WEBSOCKETEXTERNALPLAYER_H
