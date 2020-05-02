#include "seenmarkmodel.h"

void SeenMarkModel::setId(int id) noexcept
{
    m_Id = id;
}

void SeenMarkModel::setSeriaId(int seriaId) noexcept
{
    m_SeriaId = seriaId;
}

void SeenMarkModel::writeToJson(QJsonObject &json) const noexcept
{
    json["id"] = m_Id;
    json["seriaId"] = m_SeriaId;
}

void SeenMarkModel::readFromJson(QJsonValue &json)
{
    setId(json["id"].toInt());
    setSeriaId(json["seriaId"].toInt());
}
