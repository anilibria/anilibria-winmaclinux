#ifndef RELEASETORRENTMODEL_H
#define RELEASETORRENTMODEL_H

#include <QtCore>

class ReleaseTorrentModel
{

private:
    int m_Id;
    QString m_Hash;
    QString m_Quality;
    QString m_Series;
    long m_Size;
    QString m_Url;
public:
    ReleaseTorrentModel();

    void readFromApiModel(const QJsonObject &jsonObject);

    void writeToJson(QJsonObject &json) const;

    int id() const;
    void setId(const int id);

    QString hash() const;
    void setHash(const QString &hash);

    QString quality() const;
    void setQuality(const QString &quality);

    QString series() const;
    void setSeries(const QString &series);

    long size() const;
    void setSize(const long size);

    QString url() const;
    void setUrl(const QString &url);
};

#endif // RELEASETORRENTMODEL_H
