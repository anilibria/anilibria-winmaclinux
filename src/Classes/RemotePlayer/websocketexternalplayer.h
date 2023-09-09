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
    QString m_host { "" };
    int m_port { 0 };
    QString m_client { "" };

public:
    explicit WebSocketExternalPlayer(QObject *parent = nullptr, const QString& host = "", int port = 0, const QString& client = "");

    void trySetNewState(const QString& state) override;
    void trySetNewVolume(int volume) override;
    void trySetSource(const QString& path) override;
    void trySetSeek(int position) override;
    void tryMuted(bool mute) override;

private:
    void sendCommand(const QString& command, const QString& parameter);

private slots:
    void onTextMessageReceived(const QString &message);
    void onConnected();

signals:

};

#endif // WEBSOCKETEXTERNALPLAYER_H
