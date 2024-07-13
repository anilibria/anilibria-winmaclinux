#ifndef APITORRENTMODEL_H
#define APITORRENTMODEL_H

#include <QString>
#include <QJsonObject>

class ApiTorrentModel
{

private:
    int m_releaseId { 0 };
    QString m_hash { "" };
    int m_id { 0 };
    QString m_magnet { "" };
    QString m_filename { "" };
    QString m_description { "" };
    QString m_quality { "" };
    QString m_codec { "" };
    QString m_type { "" };
    int64_t m_size { 0 };
    QString m_torrentPath { "" };
    int m_seeders { 0 };
    int64_t m_created { 0 };

public:
    ApiTorrentModel();

    int releaseId() const noexcept { return m_releaseId; }
    QString hash() const noexcept { return m_hash; }
    int id() const noexcept { return m_id; }
    QString magnet() const noexcept { return m_magnet; }
    QString filename() const noexcept { return m_filename; }
    QString description() const noexcept { return m_description; }
    QString quality() const noexcept { return m_quality; }
    QString codec() const noexcept { return m_codec; }
    QString type() const noexcept { return m_type; }
    int64_t size() const noexcept { return m_size; }
    int seeders() const noexcept { return m_seeders; }
    QString torrentPath() const noexcept { return m_torrentPath; }
    int64_t created() const noexcept { return m_created; }

    void setReleaseId(int releaseId) noexcept { m_releaseId = releaseId; }
    void setHash(const QString& hash) noexcept { m_hash = hash; }
    void setId(int id) noexcept { m_id = id; }
    void setMagnet(const QString& magnet) noexcept { m_magnet = magnet; }
    void setFilename(const QString& filename) noexcept { m_filename = filename; }
    void setDescription(const QString& description) noexcept { m_description = description; }
    void setQuality(const QString& quality) noexcept { m_quality = quality; }
    void setCodec(const QString& codec) noexcept { m_codec = codec; }
    void setType(const QString& type) noexcept { m_type = type; }
    void setSize(int64_t size) noexcept { m_size = size; }
    void setTorrentPath(const QString& torrentPath) noexcept { m_torrentPath = torrentPath; }
    void setSeeders(int seeders) noexcept { m_seeders = seeders; }
    void setCreated(int64_t created) noexcept { m_created = created; }

    void readFromJson(const QJsonObject& object);

};

#endif // APITORRENTMODEL_H
