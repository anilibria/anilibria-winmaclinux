#ifndef RELEASESVIEWMODEL_H
#define RELEASESVIEWMODEL_H

#include <QObject>
#include "../ListModels/releasetorrentslist.h"
#include "imagebackgroundviewmodel.h"
#include "../Models/fullreleasemodel.h"
#include "../ListModels/releaseslistmodel.h"

class ReleasesViewModel : public QObject
{
    Q_OBJECT    
    Q_PROPERTY(ReleaseTorrentsList* openedCardTorrents READ openedCardTorrents NOTIFY openedCardTorrentsChanged)
    Q_PROPERTY(ImageBackgroundViewModel* imageBackgroundViewModel READ imageBackgroundViewModel NOTIFY imageBackgroundViewModelChanged)
    Q_PROPERTY(int countReleases READ countReleases WRITE setCountReleases NOTIFY countReleasesChanged)
    Q_PROPERTY(ReleasesListModel* items READ items NOTIFY itemsChanged)

private:
    const QString releasesCacheFileName { "releases.cache" };
    ReleaseTorrentsList* m_releaseTorrentsList { new ReleaseTorrentsList(this) };
    ImageBackgroundViewModel* m_imageBackgroundViewModel { new ImageBackgroundViewModel(this) };
    QList<FullReleaseModel*>* m_releases { new QList<FullReleaseModel*>() };
    int m_countReleases { 0 };
    ReleasesListModel* m_items;

public:
    explicit ReleasesViewModel(QObject *parent = nullptr);

    ReleaseTorrentsList* openedCardTorrents() const noexcept { return m_releaseTorrentsList; };

    ImageBackgroundViewModel* imageBackgroundViewModel() const noexcept { return m_imageBackgroundViewModel; };

    ReleasesListModel* items() const noexcept { return m_items; }

    int countReleases() const noexcept { return m_countReleases; }
    void setCountReleases(int countReleases) noexcept;

    Q_INVOKABLE QString getScheduleDay(const QString& dayNumber) const noexcept;
    Q_INVOKABLE void copyToClipboard(const QString& text) const noexcept;
    Q_INVOKABLE void copyImageToClipboard(const QString& imagePath) const;
    Q_INVOKABLE QString getVkontakteCommentPage(const QString& code) const noexcept;

private:
    void loadReleases();

signals:
    void openedCardTorrentsChanged();
    void imageBackgroundViewModelChanged();
    void countReleasesChanged();
    void itemsChanged();

};

#endif // RELEASESVIEWMODEL_H
