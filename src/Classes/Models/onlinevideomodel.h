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

    void writeToJson(QJsonObject &json) const noexcept;

    int id() const { return m_Id; }
    QString title() const { return m_Title; }
    QString sd() const { return m_SD; }
    QString hd() const noexcept { return m_HD; }
    QString fullhd() const { return m_FullHD; }
    QString sourcesd() const { return m_SourceSD; }
    QString sourcehd() const { return m_SourceHD; }

    void setId(const int id) noexcept;
    void setTitle(const QString &title) noexcept;
    void setSd(const QString &sd) noexcept;
    void setHd(const QString &hd) noexcept;
    void setFullHd(const QString &fullhd) noexcept;
    void setSourceSd(const QString &sourcesd) noexcept;
    void setSourceHd(const QString &sourcehd) noexcept;
};

#endif // ONLINEVIDEOMODEL_H
