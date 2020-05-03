#ifndef SEENMODEL_H
#define SEENMODEL_H

#include <QObject>

class SeenModel
{

private:
    int m_Id;
    int m_VideoId;
    double m_VideoPosition;
    int m_Timestamp;

public:
    int id() const {return m_Id;}
    int videoId() const {return  m_VideoId;}
    double videoPosition() const {return m_VideoPosition;}
    int timestamp() const {return m_Timestamp;}

    void setId(const int id) noexcept;
    void setVideoId(const int videoId) noexcept;
    void setVideoPosition(const double videoPosition) noexcept;
    void setTimestamp(const int timestamp) noexcept;

    void writeToJson(QJsonObject &json) const noexcept;
    void readFromJson(QJsonValue &json);
};

#endif // SEENMODEL_H
