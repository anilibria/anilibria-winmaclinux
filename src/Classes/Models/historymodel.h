#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QJsonObject>
#include <QJsonValue>

class HistoryModel
{

private:
    int m_Id;
    int m_Timestamp;
    int m_WatchTimestamp;

public:
    HistoryModel();

    int id() const {return m_Id;}
    int timestamp() const {return m_Timestamp;}
    int watchTimestamp() const {return m_WatchTimestamp;}

    void setId(const int id) noexcept;
    void setTimestamp(const int timestamp) noexcept;
    void setWatchTimestamp(const int watchTimestamp) noexcept;

    void writeToJson(QJsonObject &json) const noexcept;
    void readFromJson(QJsonValue &json);

};

#endif // HISTORYMODEL_H
