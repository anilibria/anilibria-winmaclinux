#ifndef EXTERNALPLAYLISTVIDEO_H
#define EXTERNALPLAYLISTVIDEO_H

#include <QString>

class ExternalPlaylistVideo
{

private:
    int m_Order;
    QString m_Address;
    QString m_Name;

public:
    ExternalPlaylistVideo();

    int order() const { return m_Order; }
    QString address() const { return m_Address; }
    QString name() const { return m_Name; }

    void setOrder(const int order) noexcept;
    void setAddress(const QString& address) noexcept;
    void setName(const QString& name) noexcept;

    QString exportToM3U() noexcept;
    QString exportToMPCPL() noexcept;

    bool operator== (const ExternalPlaylistVideo &comparedModel) noexcept;
};

#endif // EXTERNALPLAYLISTVIDEO_H
