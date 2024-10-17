#ifndef EXTENSIONSVIEWMODEL_H
#define EXTENSIONSVIEWMODEL_H

#include <QObject>
#include <QJSValue>
#include <QMap>
#include <QMultiMap>
#include <QJSEngine>
#include <QList>
#include <QNetworkAccessManager>
#include "releasesviewmodel.h"
#include "extensioninnerobject.h"

class ExtensionsViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList displayedExtensions READ displayedExtensions NOTIFY displayedExtensionsChanged FINAL)
    Q_PROPERTY(ReleasesViewModel* releases READ releases WRITE setReleases NOTIFY releasesChanged FINAL)
    Q_PROPERTY(QVariantList globalVariables READ globalVariables NOTIFY globalVariablesChanged FINAL)
    Q_PROPERTY(bool isOpenedCard READ isOpenedCard NOTIFY isOpenedCardChanged FINAL)

private:
    QList<QString> m_extensions { QList<QString>() };
    ExtensionInnerObject* m_innerObject { new ExtensionInnerObject(this) };
    QMap<QString, QJSValue> m_importedModules { QMap<QString, QJSValue>() };
    QMap<QString, std::tuple<QString, QString>> m_importedModulesMetadata { QMap<QString, std::tuple<QString, QString>>() };
    QMap<QString, std::tuple<int, int>> m_importedModulesMenuIndexes { QMap<QString, std::tuple<int, int>>() };
    QList<QJSValue> m_importedModulesMenuCallbacks { QList<QJSValue>() };
    QList<QString> m_importedModulesMenuTitles { QList<QString>() };
    QJSEngine* m_engine { new QJSEngine(this) };
    QMap<QString, QJSValue> m_pendingCallbacks { QMap<QString, QJSValue>() };
    QNetworkAccessManager* m_networkManager { new QNetworkAccessManager(this) };
    QMap<QString, QString>* m_values { new QMap<QString, QString>() };
    QString m_valuesFileName { "extensionvalues.cache" };
    QString m_extensionsFileName { "extensions.cache" };
    QVariantList m_displayedExtensions { QVariantList() };
    ReleasesViewModel* m_releases { nullptr };
    QVariantList m_globalVariables { QVariantList() };
    bool m_isOpenedCard { false };

public:
    explicit ExtensionsViewModel(QObject *parent = nullptr);

    QVariantList displayedExtensions() const noexcept { return m_displayedExtensions; }

    ReleasesViewModel* releases() const noexcept { return m_releases; }
    void setReleases(const ReleasesViewModel* releases) noexcept;

    QVariantList globalVariables() const noexcept { return m_globalVariables; }

    bool isOpenedCard() const noexcept { return m_isOpenedCard; }

    Q_INVOKABLE void releaseOpenedInVideoPlayer(int releaseId, const QString& title, int seria);
    Q_INVOKABLE void saveValue(const QString& key, const QString& value);
    Q_INVOKABLE QString readValue(const QString& key);
    Q_INVOKABLE void deleteValue(const QString& key);
    Q_INVOKABLE void addExtension(const QString& path);
    Q_INVOKABLE void runMenuCommand(const QString& identifier, int index);
    Q_INVOKABLE void saveValues();
    Q_INVOKABLE void deleteExtension(const QString& path);
    Q_INVOKABLE void openCard();
    Q_INVOKABLE void closeCard();
    Q_INVOKABLE void refreshVariables();
    Q_INVOKABLE bool hasValue(const QString& key) noexcept;

private:
    void importExtensions();
    void importExtension(const QString& path);
    void adjustEngine();
    void readValues();
    void readExtensions();
    void saveExtensions();
    void remapDisplayExtensions();
    void refreshGlobalVariables();

private slots:
    void requestFinished(QNetworkReply *reply);
    void makeHttpGet(const QString& url, const QList<QString> headers, const QJSValue& callback);
    void makeHttpDelete(const QString& url, const QList<QString> headers, const QJSValue& callback);
    void makeHttpPost(const QString& url, const QList<QString> headers, const QString& body, const QJSValue& callback);
    void makeHttpPut(const QString& url, const QList<QString> headers, const QString& body, const QJSValue& callback);
    void sendInformationNotification(const QString& message);

signals:
    void displayedExtensionsChanged();
    void releasesChanged();
    void globalVariablesChanged();
    void isOpenedCardChanged();
    void sendInformationNotificationReceived(const QString& message);

};

#endif // EXTENSIONSVIEWMODEL_H
