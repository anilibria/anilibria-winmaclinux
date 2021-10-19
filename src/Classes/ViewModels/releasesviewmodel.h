#ifndef RELEASESVIEWMODEL_H
#define RELEASESVIEWMODEL_H

#include <QObject>
#include "../ListModels/releasetorrentslist.h"

class ReleasesViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ReleaseTorrentsList* openedCardTorrents READ openedCardTorrents NOTIFY openedCardTorrentsChanged)

private:
    ReleaseTorrentsList* m_releaseTorrentsList { new ReleaseTorrentsList(this) };

public:
    explicit ReleasesViewModel(QObject *parent = nullptr);

    ReleaseTorrentsList* openedCardTorrents() const noexcept { return m_releaseTorrentsList; };

    Q_INVOKABLE QString getScheduleDay(const QString& dayNumber) const noexcept;
    Q_INVOKABLE void copyToClipboard(const QString& text) const noexcept;
    Q_INVOKABLE void copyImageToClipboard(const QString& imagePath) const;

signals:
    void openedCardTorrentsChanged();

};

#endif // RELEASESVIEWMODEL_H
