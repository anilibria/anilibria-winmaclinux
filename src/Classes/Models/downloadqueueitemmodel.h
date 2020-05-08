#ifndef DOWNLOADQUEUEITEMMODEL_H
#define DOWNLOADQUEUEITEMMODEL_H

#include <QString>

class DownloadQueueItemModel
{

private:
    int m_Id;
    int m_SeriaId;
    QString m_Url;

public:
    DownloadQueueItemModel() noexcept;

    int id() const { return m_Id; }
    int seriaId() const { return m_SeriaId; }
    QString url() const { return m_Url; }

    void setId(int id) noexcept;
    void setSeriaId(int seriaId) noexcept;
    void setUrl(const QString& url) noexcept;

};

#endif // DOWNLOADQUEUEITEMMODEL_H
