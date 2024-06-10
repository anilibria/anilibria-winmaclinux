#ifndef EXTENSIONSVIEWMODEL_H
#define EXTENSIONSVIEWMODEL_H

#include <QObject>
#include <QJSValue>
#include <QMap>
#include <QJSEngine>
#include <QList>
#include <QNetworkAccessManager>

class ExtensionsViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList displayedExtensions READ displayedExtensions NOTIFY displayedExtensionsChanged FINAL)

private:
    QList<QString> m_extensions { QList<QString>() };
    QMap<QString, QJSValue> m_importedModules { QMap<QString, QJSValue>() };
    QMap<QString, std::tuple<QString, QString>> m_importedModulesMetadata { QMap<QString, std::tuple<QString, QString>>() };
    QJSEngine* m_engine { new QJSEngine(this) };
    QMap<QString, QJSValue> m_pendingCallbacks { QMap<QString, QJSValue>() };
    QNetworkAccessManager* m_networkManager { new QNetworkAccessManager(this) };
    QMap<QString, QString> m_values { QMap<QString, QString>() };
    QString m_valuesPath { "" };
    QString m_extensionsPath { "" };
    QVariantList m_displayedExtensions { QVariantList() };

public:
    explicit ExtensionsViewModel(QObject *parent = nullptr);

    QVariantList displayedExtensions() const noexcept { return m_displayedExtensions; }

    Q_INVOKABLE void releaseOpenedInVideoPlayer(int releaseId, const QString& title, int seria);
    Q_INVOKABLE void makeHttpGet(const QString& url, const QList<QString> headers, const QJSValue& callback);
    Q_INVOKABLE void log(const QString& message);
    Q_INVOKABLE void saveValue(const QString& key, const QString& value);
    Q_INVOKABLE QString readValue(const QString& key);
    Q_INVOKABLE void deleteValue(const QString& key);
    Q_INVOKABLE void addExtension(const QString& path);

private:
    void importExtensions();
    void importExtension(const QString& path);
    void adjustEngine();
    void readValues();
    void saveValues();
    void readExtensions();
    void saveExtensions();
    void remapDisplayExtensions();

private slots:
    void requestFinished(QNetworkReply *reply);

signals:
    void displayedExtensionsChanged();

};

#endif // EXTENSIONSVIEWMODEL_H
