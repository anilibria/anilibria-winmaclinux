#include "seenmarkmodel.h"

int SeenMarkModel::id() const
{
    return m_Id;
}

int SeenMarkModel::seriaId() const
{
    return m_SeriaId;
}

void SeenMarkModel::setId(int id)
{
    m_Id = id;
}

void SeenMarkModel::setSeriaId(int seriaId)
{
    m_SeriaId = seriaId;
}

void SeenMarkModel::writeToJson(QJsonObject &json) const
{
    json["id"] = m_Id;
    json["seriaId"] = m_SeriaId;
}

void SeenMarkModel::readFromJson(QJsonValue &json)
{
    setId(json["id"].toInt());
    setSeriaId(json["seriaId"].toInt());
}
