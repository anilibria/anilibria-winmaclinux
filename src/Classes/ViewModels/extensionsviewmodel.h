#ifndef EXTENSIONSVIEWMODEL_H
#define EXTENSIONSVIEWMODEL_H

#include <QObject>
#include <QJSValue>
#include <QMap>
#include <QJSEngine>
#include <QNetworkAccessManager>

class ExtensionsViewModel : public QObject
{
    Q_OBJECT

private:
    QList<QString> m_extensions { QList<QString>() };
    QMap<QString, QJSValue> m_importedModules { QMap<QString, QJSValue>() };
    QMap<QString, std::tuple<QString, QString>> m_importedModulesMetadata { QMap<QString, std::tuple<QString, QString>>() };
    QJSEngine* m_engine { new QJSEngine(this) };
    QMap<QString, QJSValue> m_pendingCallbacks { QMap<QString, QJSValue>() };
    QNetworkAccessManager* m_networkManager { new QNetworkAccessManager(this) };
    QMap<QString, QString> m_values { QMap<QString, QString>() };

public:
    explicit ExtensionsViewModel(QObject *parent = nullptr);

    Q_INVOKABLE void releaseOpenedInVideoPlayer(int releaseId, const QString& title, int seria);
    Q_INVOKABLE void makeHttpGet(const QString& url, const QList<QString> headers, const QJSValue& callback);
    Q_INVOKABLE void log(const QString& message);
    Q_INVOKABLE void saveValue(const QString& key, const QString& value);
    Q_INVOKABLE QString readValue(const QString& key);
    Q_INVOKABLE void deleteValue(const QString& key);

private:
    void importExtensions();
    void adjustEngine();

private slots:
    void requestFinished(QNetworkReply *reply);

signals:

};

#endif // EXTENSIONSVIEWMODEL_H
