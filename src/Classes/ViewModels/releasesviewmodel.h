#ifndef RELEASESVIEWMODEL_H
#define RELEASESVIEWMODEL_H

#include <QObject>
#include "../ListModels/releasetorrentslist.h"
#include "imagebackgroundviewmodel.h"

class ReleasesViewModel : public QObject
{
    Q_OBJECT    
    Q_PROPERTY(ReleaseTorrentsList* openedCardTorrents READ openedCardTorrents NOTIFY openedCardTorrentsChanged)
    Q_PROPERTY(ImageBackgroundViewModel* imageBackgroundViewModel READ imageBackgroundViewModel NOTIFY imageBackgroundViewModelChanged)

private:
    ReleaseTorrentsList* m_releaseTorrentsList { new ReleaseTorrentsList(this) };
    ImageBackgroundViewModel* m_imageBackgroundViewModel { new ImageBackgroundViewModel(this) };

public:
    explicit ReleasesViewModel(QObject *parent = nullptr);

    ReleaseTorrentsList* openedCardTorrents() const noexcept { return m_releaseTorrentsList; };

    ImageBackgroundViewModel* imageBackgroundViewModel() const noexcept { return m_imageBackgroundViewModel; };

    Q_INVOKABLE QString getScheduleDay(const QString& dayNumber) const noexcept;
    Q_INVOKABLE void copyToClipboard(const QString& text) const noexcept;
    Q_INVOKABLE void copyImageToClipboard(const QString& imagePath) const;
    Q_INVOKABLE QString getVkontakteCommentPage(const QString& code) const noexcept;

signals:
    void openedCardTorrentsChanged();
    void imageBackgroundViewModelChanged();

};

#endif // RELEASESVIEWMODEL_H
