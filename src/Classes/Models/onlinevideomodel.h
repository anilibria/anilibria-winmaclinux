#ifndef ONLINEVIDEOMODEL_H
#define ONLINEVIDEOMODEL_H

#include <QtCore>

class OnlineVideoModel
{
private:
    int m_Id;
    QString m_Title;
    QString m_SD;
    QString m_HD;
    QString m_FullHD;
    QString m_SourceSD;
    QString m_SourceHD;

public:
    OnlineVideoModel();

    void readFromApiModel(const QJsonObject &jsonObject);

    void writeToJson(QJsonObject &json) const;

    int id() const;
    QString title() const;
    QString sd() const;
    QString hd() const;
    QString fullhd() const;
    QString sourcesd() const;
    QString sourcehd() const;

    void setId(const int id);
    void setTitle(const QString &title);
    void setSd(const QString &sd);
    void setHd(const QString &hd);
    void setFullHd(const QString &fullhd);
    void setSourceSd(const QString &sourcesd);
    void setSourceHd(const QString &sourcehd);
};

#endif // ONLINEVIDEOMODEL_H
