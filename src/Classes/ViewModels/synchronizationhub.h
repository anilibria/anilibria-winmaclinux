#ifndef SYNCHRONIZATIONHUB_H
#define SYNCHRONIZATIONHUB_H

#include <QObject>
#include "releasesviewmodel.h"
#include "../Services/synchronizev2service.h"

class SynchronizationHub : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ReleasesViewModel* releases READ releases WRITE setReleases NOTIFY releasesChanged FINAL)
    Q_PROPERTY(Synchronizev2Service* synchronization READ synchronization WRITE setSynchronization NOTIFY synchronizationChanged FINAL)

private:
    ReleasesViewModel* m_releases;
    Synchronizev2Service* m_synchronization;

public:
    explicit SynchronizationHub(QObject *parent = nullptr);

    ReleasesViewModel* releases() const noexcept { return m_releases; }
    void setReleases(const ReleasesViewModel* releases);

    Synchronizev2Service* synchronization() const noexcept { return m_synchronization; }
    void setSynchronization(Synchronizev2Service* synchronization);

    Q_INVOKABLE void addSelectedReleasesToCollection(const QString& collection);
    Q_INVOKABLE void deleteSelectedReleasesFromCollections();

private slots:
    void synchronizeCollections();

signals:
    void releasesChanged();
    void synchronizationChanged();

};

#endif // SYNCHRONIZATIONHUB_H
