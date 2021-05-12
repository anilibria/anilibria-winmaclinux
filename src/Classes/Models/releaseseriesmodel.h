#ifndef RELEASESERIESMODEL_H
#define RELEASESERIESMODEL_H

#include <QObject>
#include <QJsonObject>

class ReleaseSeriesModel
{

private:
    QList<QVariant>* m_releaseIds;
    qint32 m_countReleases;
    QString m_firstName;
    QString m_secondName;
    QString m_thirdName;

public:
    explicit ReleaseSeriesModel();

    QList<QVariant>* releaseIds() const { return m_releaseIds; }
    qint32 countReleases() const { return m_countReleases; }
    QString firstName() const { return m_firstName; }
    QString secondName() const { return m_secondName; }
    QString thirdName() const { return m_thirdName; }

    void recalculateCountReleases();
    void setFirstName(const QString& firstname);
    void setSecondName(const QString& secondname);
    void setThirdName(const QString& thirdname);
    bool appendReleaseId(const int id);

    void readFromJson(const QJsonObject &jsonObject) noexcept;
    void writeToJson(QJsonObject &json) const noexcept;
};

#endif // RELEASESERIESMODEL_H
