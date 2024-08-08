#include <QDebug>
#include "extensioninnerobject.h"

ExtensionInnerObject::ExtensionInnerObject(QObject *parent)
    : QObject{parent}
{}

void ExtensionInnerObject::setup(QMap<QString, QString> *values) noexcept
{
    m_values = values;
}

void ExtensionInnerObject::makeHttpGet(const QString &url, const QList<QString> headers, const QJSValue &callback)
{
    emit makeHttpGetHandler(url, headers, callback);
}

void ExtensionInnerObject::makeHttpPost(const QString &url, const QList<QString> headers, const QString &body, const QJSValue &callback)
{
    emit makeHttpPostHandler(url, headers, body, callback);
}

void ExtensionInnerObject::log(const QString &message)
{
    qDebug() << "[extension]: " << message;
}

void ExtensionInnerObject::saveValue(const QString &key, const QString &value)
{
    m_values->insert(key, value);
}

QString ExtensionInnerObject::readValue(const QString &key)
{
    if (!m_values->contains(key)) return "";

    return m_values->value(key);
}
