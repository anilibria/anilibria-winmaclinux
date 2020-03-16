#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QObject>
#include <QSettings>

class ApplicationSettings : public QObject
{
    Q_OBJECT


    Q_PROPERTY(QString userToken READ userToken WRITE setUserToken NOTIFY userTokenChanged)

private:
    QSettings* m_Settings;

public:
    explicit ApplicationSettings(QObject *parent = nullptr);

    QString userToken();
    void setUserToken(QString& token);

signals:
    void userTokenChanged(QString& token);

};

#endif // APPLICATIONSETTINGS_H
