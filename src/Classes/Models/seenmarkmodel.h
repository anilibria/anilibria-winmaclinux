#ifndef SEENMARKMODEL_H
#define SEENMARKMODEL_H

#include <QJsonObject>
#include <QJsonValue>

class SeenMarkModel
{

private:
    int m_Id;
    int m_SeriaId;

public:
    int id() const;
    int seriaId() const;

    void setId(int id);
    void setSeriaId(int seriaId);

    void writeToJson(QJsonObject &json) const;
    void readFromJson(QJsonValue &json);

};

#endif // SEENMARKMODEL_H
