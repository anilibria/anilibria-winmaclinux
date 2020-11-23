#ifndef APISERVICECONFIGURATOR_H
#define APISERVICECONFIGURATOR_H

#include <QObject>

class ApiServiceConfigurator : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isDefault READ isDefault NOTIFY isDefaultChanged)
    Q_PROPERTY(QString apiAddress READ apiAddress NOTIFY apiAddressChanged)
    Q_PROPERTY(QString staticAddress READ staticAddress NOTIFY staticAddressChanged)
private:
    bool m_IsDefault;
    QString m_ApiAddress;
    QString m_StaticAddress;

public:
    explicit ApiServiceConfigurator(QObject *parent = nullptr);

    bool isDefault() const { return m_IsDefault; }
    QString apiAddress() const { return m_ApiAddress; }
    QString staticAddress() const { return m_StaticAddress; }

    Q_INVOKABLE void saveApiConfiguration(const QString& apiAddress, const QString& staticAddress);
    Q_INVOKABLE void restoreDefault();

private:
    void setConfiguration(const QString& apiAddress, const QString& staticAddress);
    QString getConfigurationPath();

signals:
    void isDefaultChanged();
    void apiAddressChanged();
    void staticAddressChanged();

};

#endif // APISERVICECONFIGURATOR_H
