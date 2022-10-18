#ifndef THEMEITEMMODEL_H
#define THEMEITEMMODEL_H

#include <QString>
#include <QJsonObject>

class ThemeItemModel
{

private:
    QString m_name { "" };
    QString m_author { "" };
    QString m_screenshoot { "" };
    QString m_source { "" };
    int m_version { 0 };
    int m_lastUpdated { 0 };

public:
    ThemeItemModel();

    QString name() const noexcept { return m_name; }
    QString author() const noexcept { return m_author; }
    QString screenshoot() const noexcept { return m_screenshoot; }
    QString source() const noexcept { return m_source; }
    int version() const noexcept { return m_version; }
    int lastUpdated() const noexcept { return m_lastUpdated; }

    void setName(const QString& name) noexcept { m_name = name; }
    void setAuthor(const QString& author) noexcept { m_author = author; }
    void setScreenshoot(const QString& screenshoot) noexcept { m_screenshoot = screenshoot; }
    void setSource(const QString& source) noexcept { m_source = source; }
    void setVersion(int version) noexcept { m_version = version; }
    void setLastUpdated(int lastUpdated) noexcept { m_lastUpdated = lastUpdated; }

    void readFromJson(const QJsonObject& object);

};

#endif // THEMEITEMMODEL_H
