#ifndef EXTENSIONINNEROBJECT_H
#define EXTENSIONINNEROBJECT_H

#include <QObject>
#include <QJSValue>
#include <QMap>
#include "releasesviewmodel.h"

class ExtensionInnerObject : public QObject
{
    Q_OBJECT
private:
    QMap<QString, QString> *m_values { nullptr };
    ReleasesViewModel *m_releases { nullptr };

public:
    explicit ExtensionInnerObject(QObject *parent = nullptr);

    void setup(QMap<QString, QString>* values) noexcept;
    void setupReleases(const ReleasesViewModel* releases) noexcept;

    Q_INVOKABLE void makeHttpGet(const QString& url, const QList<QString> headers, const QJSValue& callback);
    Q_INVOKABLE void makeHttpPost(const QString& url, const QList<QString> headers, const QString& body, const QJSValue& callback);
    Q_INVOKABLE void makeHttpPut(const QString& url, const QList<QString> headers, const QString& body, const QJSValue& callback);
    Q_INVOKABLE void makeHttpDelete(const QString& url, const QList<QString> headers, const QJSValue& callback);
    Q_INVOKABLE void log(const QString& message);
    Q_INVOKABLE void saveValue(const QString& key, const QString& value);
    Q_INVOKABLE QString readValue(const QString& key);
    Q_INVOKABLE QVariant getRelease(int id);
    Q_INVOKABLE QList<int> getFavorites();
    Q_INVOKABLE QList<int> getSeens();
    Q_INVOKABLE int getReleasesCount();
    Q_INVOKABLE void sendInformationNotification(const QString& message);

signals:
    void makeHttpGetHandler(const QString& url, const QList<QString> headers, const QJSValue& callback);
    void makeHttpPostHandler(const QString& url, const QList<QString> headers, const QString& body, const QJSValue& callback);
    void makeHttpPutHandler(const QString& url, const QList<QString> headers, const QString& body, const QJSValue& callback);
    void makeHttpDeleteHandler(const QString& url, const QList<QString> headers, const QJSValue& callback);
    void sendInformationNotificationHandler(const QString& message);

};

#endif // EXTENSIONINNEROBJECT_H
