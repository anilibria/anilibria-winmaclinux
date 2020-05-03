#ifndef YOUTUBEVIDEOMODEL_H
#define YOUTUBEVIDEOMODEL_H

#include <QString>
#include <QJsonObject>
#include <QJsonValue>

class YoutubeVideoModel
{

private:
    int m_Id;
    QString m_Title;
    QString m_Image;
    QString m_VideoIdentifier;
    int m_Timestamp;

public:
    YoutubeVideoModel();

    int id() const { return m_Id; }
    QString title() const { return m_Title; }
    QString image() const { return m_Image; }
    QString videoIdentifier() const { return  m_VideoIdentifier; }
    int timestamp() const { return m_Timestamp; }

    void setId(const int id) noexcept;
    void setTitle(const QString& title) noexcept;
    void setImage(const QString& image) noexcept;
    void setVideoIdentifier(const QString& videoIdentifier) noexcept;
    void setTimestamp(const int timestamp) noexcept;

    void writeToJson(QJsonObject &json) const noexcept;
    void readFromJson(QJsonValue &json);

};

#endif // YOUTUBEVIDEOMODEL_H
