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
    int id() const { return m_Id; }
    int seriaId() const { return m_SeriaId; }

    void setId(const int id) noexcept;
    void setSeriaId(const int seriaId) noexcept;

    void writeToJson(QJsonObject &json) const noexcept;
    void readFromJson(QJsonValue &json);

};

#endif // SEENMARKMODEL_H
