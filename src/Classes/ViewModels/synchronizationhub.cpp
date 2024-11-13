#include "synchronizationhub.h"

SynchronizationHub::SynchronizationHub(QObject *parent)
    : QObject{parent}
{
}

void SynchronizationHub::setReleases(const ReleasesViewModel *releases)
{
    if (m_releases == releases) return;

    m_releases = const_cast<ReleasesViewModel *>(releases);
    emit releasesChanged();


}

void SynchronizationHub::setSynchronization(Synchronizev2Service *synchronization)
{
    if (m_synchronization == synchronization) return;

    m_synchronization = const_cast<Synchronizev2Service *>(synchronization);
    emit releasesChanged();

    connect(m_synchronization, &Synchronizev2Service::synchronizeCollectionCompleted, this, &SynchronizationHub::synchronizeCollections);
}

void SynchronizationHub::addSelectedReleasesToCollection(const QString &collection)
{
    auto releaseIds = m_releases->items()->getSelectedReleases();
    QList<int> ids(releaseIds->cbegin(), releaseIds->cend());
    m_synchronization->addReleasesToCollection(ids, collection);
    m_releases->setReleasesToCollection(ids, collection);
    m_releases->clearSelectedReleases();
}

void SynchronizationHub::deleteSelectedReleasesFromCollections()
{
    auto releaseIds = m_releases->items()->getSelectedReleases();
    QList<int> ids(releaseIds->cbegin(), releaseIds->cend());

    m_synchronization->removeReleasesFromCollection(ids);
    m_releases->removeReleasesFromCollections(ids);
    m_releases->clearSelectedReleases();
}

void SynchronizationHub::synchronizeCollections()
{
    auto collections = m_synchronization->getLocalCollections();
    m_releases->synchronizeCollections(std::move(collections));
}

