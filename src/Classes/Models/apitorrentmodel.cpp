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
    if (object.contains("quality")) m_quality = object.value("quality").toString();
    if (object.contains("codec")) m_codec = object.value("codec").toString();
    if (object.contains("size")) m_size = object.value("size").toInt(0);
}
