#ifndef APPLICATIONVERSIONCHECKER_H
#define APPLICATIONVERSIONCHECKER_H

#include <QObject>
#include <QNetworkReply>

class ApplicationVersionChecker : public QObject
{
    Q_OBJECT

private:
    QNetworkAccessManager * m_manager { new QNetworkAccessManager(this) };
    QMap<QString, QString> m_currentChecks { QMap<QString, QString>() };

public:
    explicit ApplicationVersionChecker(QObject *parent = nullptr);
    void checkNewVersionAvailable(QString appIdentifier, QString repository, QString currentVersion);

signals:
    void newVersionAvailable(QString version, QString url, QString appIdentifier);

public slots:
    void downloaded(QNetworkReply* reply);

};

#endif // APPLICATIONVERSIONCHECKER_H
