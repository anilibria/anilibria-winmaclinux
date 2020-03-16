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

    int id() const;
    int timestamp() const;
    int watchTimestamp() const;

    void setId(int id);
    void setTimestamp(int timestamp);
    void setWatchTimestamp(int watchTimestamp);

    void writeToJson(QJsonObject &json) const;
    void readFromJson(QJsonValue &json);

};

#endif // HISTORYMODEL_H
