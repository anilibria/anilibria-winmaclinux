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

void ReleaseTorrentModel::writeToJson(QJsonObject &json) const
{
    json["id"] = m_Id;
    json["hash"] = m_Hash;
    json["quality"] = m_Quality;
    json["series"] = m_Series;
    json["url"] = m_Url;
    json["size"] = static_cast<int>(m_Size);
}

int ReleaseTorrentModel::id() const
{
    return m_Id;
}

void ReleaseTorrentModel::setId(const int id)
{
    m_Id = id;
}

QString ReleaseTorrentModel::hash() const
{
    return m_Hash;
}

void ReleaseTorrentModel::setHash(const QString &hash)
{
    m_Hash = hash;
}

QString ReleaseTorrentModel::quality() const
{
    return m_Quality;
}

void ReleaseTorrentModel::setQuality(const QString &quality)
{
    m_Quality = quality;
}

QString ReleaseTorrentModel::series() const
{
    return m_Series;
}

void ReleaseTorrentModel::setSeries(const QString &series)
{
    m_Series = series;
}

long ReleaseTorrentModel::size() const
{
    return m_Size;
}

void ReleaseTorrentModel::setSize(const long size)
{
    m_Size = size;
}

QString ReleaseTorrentModel::url() const
{
    return m_Url;
}

void ReleaseTorrentModel::setUrl(const QString &url)
{
    m_Url = url;
}
