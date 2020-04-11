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

    int order() const;
    QString address() const;
    QString name() const;

    void setOrder(int order);
    void setAddress(const QString& address);
    void setName(const QString& name);

    QString exportToM3U();
    QString exportToMPCPL();

    bool operator== (const ExternalPlaylistVideo &comparedModel);
};

#endif // EXTERNALPLAYLISTVIDEO_H
