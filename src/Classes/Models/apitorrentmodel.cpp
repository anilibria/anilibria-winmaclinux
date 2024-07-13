#include "apitorrentmodel.h"

ApiTorrentModel::ApiTorrentModel() {

}

void ApiTorrentModel::readFromJson(const QJsonObject &object)
{
    if (object.contains("releaseId")) m_releaseId = object.value("releaseId").toInt(0);
    if (object.contains("hash")) m_hash = object.value("hash").toString();
    if (object.contains("id")) m_id = object.value("id").toInt(0);
    if (object.contains("magnet")) m_magnet = object.value("magnet").toString();
    if (object.contains("filename")) m_filename = object.value("filename").toString();
    if (object.contains("description")) m_description = object.value("description").toString();
    if (object.contains("quality")) {
        auto qualityObject = object.value("quality").toObject();
        if (qualityObject.contains("description")) m_quality = qualityObject.value("description").toString();
    }
    if (object.contains("codec")) {
        auto codecObject = object.value("codec").toObject();
        if (codecObject.contains("description")) m_codec = codecObject.value("description").toString();
    }
    if (object.contains("type")) {
        auto typeObject = object.value("type").toObject();
        if (typeObject.contains("description")) m_type = typeObject.value("description").toString();
    }
    if (object.contains("size")) {
        auto value = object.value("size").toVariant();
        m_size = value.toString().toLongLong();
    }
    if (object.contains("seeders")) m_seeders = object.value("seeders").toInt(0);
    if (object.contains("time")) m_created = object.value("time").toInt(0);
    if (m_id != 0) m_torrentPath = QString("/api/v1/anime/torrents/") + QString::number(m_id) + QString("/file");
}
