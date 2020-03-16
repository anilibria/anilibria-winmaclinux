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

    int id();
    QString title();
    QString image();
    QString videoIdentifier();
    int timestamp();

    void setId(int id);
    void setTitle(QString title);
    void setImage(QString image);
    void setVideoIdentifier(QString videoIdentifier);
    void setTimestamp(int timestamp);

    void writeToJson(QJsonObject &json) const;
    void readFromJson(QJsonValue &json);

};

#endif // YOUTUBEVIDEOMODEL_H
