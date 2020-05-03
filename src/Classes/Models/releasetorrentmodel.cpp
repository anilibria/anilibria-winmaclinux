#include "releasetorrentmodel.h"

ReleaseTorrentModel::ReleaseTorrentModel()
{

}

void ReleaseTorrentModel::readFromApiModel(const QJsonObject &jsonObject)
{
    m_Id = jsonObject.value("id").toInt();
    m_Hash = jsonObject.value("hash").toString();
    m_Quality = jsonObject.value("quality").toString();
    m_Series = jsonObject.value("series").toString();
    m_Url = jsonObject.value("url").toString();
    m_Size = jsonObject.value("size").toInt();
}

void ReleaseTorrentModel::writeToJson(QJsonObject &json) const noexcept
{
    json["id"] = m_Id;
    json["hash"] = m_Hash;
    json["quality"] = m_Quality;
    json["series"] = m_Series;
    json["url"] = m_Url;
    json["size"] = static_cast<int>(m_Size);
}

void ReleaseTorrentModel::setId(const int id) noexcept
{
    m_Id = id;
}

void ReleaseTorrentModel::setHash(const QString &hash) noexcept
{
    m_Hash = hash;
}

void ReleaseTorrentModel::setQuality(const QString &quality) noexcept
{
    m_Quality = quality;
}

void ReleaseTorrentModel::setSeries(const QString &series) noexcept
{
    m_Series = series;
}

void ReleaseTorrentModel::setSize(const long size) noexcept
{
    m_Size = size;
}

void ReleaseTorrentModel::setUrl(const QString &url) noexcept
{
    m_Url = url;
}
