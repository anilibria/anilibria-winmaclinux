#ifndef EXTENSIONINNEROBJECT_H
#define EXTENSIONINNEROBJECT_H

#include <QObject>
#include <QJSValue>
#include <QMap>

class ExtensionInnerObject : public QObject
{
    Q_OBJECT
private:
    QMap<QString, QString> *m_values { nullptr };

public:
    explicit ExtensionInnerObject(QObject *parent = nullptr);

    void setup(QMap<QString, QString>* values) noexcept;

    Q_INVOKABLE void makeHttpGet(const QString& url, const QList<QString> headers, const QJSValue& callback);
    Q_INVOKABLE void makeHttpPost(const QString& url, const QList<QString> headers, const QString& body, const QJSValue& callback);
    Q_INVOKABLE void log(const QString& message);
    Q_INVOKABLE void saveValue(const QString& key, const QString& value);
    Q_INVOKABLE QString readValue(const QString& key);

signals:
    void makeHttpGetHandler(const QString& url, const QList<QString> headers, const QJSValue& callback);
    void makeHttpPostHandler(const QString& url, const QList<QString> headers, const QString& body, const QJSValue& callback);

};

#endif // EXTENSIONINNEROBJECT_H
