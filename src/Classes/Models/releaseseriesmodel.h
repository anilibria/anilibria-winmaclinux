#ifndef RELEASESERIESMODEL_H
#define RELEASESERIESMODEL_H

#include <QObject>
#include <QJsonObject>

class ReleaseSeriesModel
{

private:
    QList<QVariant>* m_releaseIds;
    QList<QVariant>* m_posters;
    qint32 m_countReleases;
    QString m_firstName;
    QString m_secondName;
    QString m_thirdName;
    QString m_firstPoster;
    QString m_secondPoster;
    QString m_thirdPoster;

public:
    explicit ReleaseSeriesModel();

    QList<QVariant>* releaseIds() const { return m_releaseIds; }
    QList<QVariant>* posters() const { return m_posters; }
    qint32 countReleases() const { return m_countReleases; }
    QString firstName() const { return m_firstName; }
    QString secondName() const { return m_secondName; }
    QString thirdName() const { return m_thirdName; }
    QString firstPoster() const { return m_firstPoster; }
    QString secondPoster() const { return m_secondPoster; }
    QString thirdPoster() const { return m_thirdPoster; }

    void recalculateCountReleases();
    void setFirstName(const QString& firstname);
    void setSecondName(const QString& secondname);
    void setThirdName(const QString& thirdname);
    void setFirstPoster(const QString& firstposter);
    void setSecondPoster(const QString& secondposter);
    void setThirdPoster(const QString& thirdposter);
    bool appendReleaseId(const int id);
    void appendPoster(const QString& poster);

    void readFromJson(const QJsonObject &jsonObject) noexcept;
    void writeToJson(QJsonObject &json) const noexcept;
};

#endif // RELEASESERIESMODEL_H
