#include "downloadqueueitemmodel.h"

DownloadQueueItemModel::DownloadQueueItemModel() noexcept : m_Id(0), m_SeriaId(0), m_Url("")
{

}

void DownloadQueueItemModel::setId(int id) noexcept
{
    m_Id = id;
}

void DownloadQueueItemModel::setSeriaId(int seriaId) noexcept
{
    m_SeriaId = seriaId;
}

void DownloadQueueItemModel::setUrl(const QString &url) noexcept
{
    m_Url = url;
}
