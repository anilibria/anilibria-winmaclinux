#ifndef RELEASECUSTOMGROUPSVIEWMODEL_H
#define RELEASECUSTOMGROUPSVIEWMODEL_H

#include <QObject>
#include <QMap>
#include <QMultiMap>
#include <QSet>
#include <QVariantList>

class ReleaseCustomGroupsViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList groups READ groups NOTIFY groupsChanged)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(bool hasGroups READ hasGroups NOTIFY hasGroupsChanged)
    Q_PROPERTY(bool groupEditMode READ groupEditMode NOTIFY groupEditModeChanged)
    Q_PROPERTY(QVariantList releaseNotUsedGroups READ releaseNotUsedGroups NOTIFY releaseNotUsedGroupsChanged)
    Q_PROPERTY(QVariantList releaseUsedGroups READ releaseUsedGroups NOTIFY releaseUsedGroupsChanged)
    Q_PROPERTY(QString releaseGroupsLink READ releaseGroupsLink NOTIFY releaseGroupsLinkChanged)

private:
    const QString groupsCacheFileName { "groups.cache" };
    QMap<int, QString> m_groupNames { QMap<int, QString>() };
    QMultiMap<int, int> m_groupValues { QMultiMap<int, int>() };
    QVariantList m_groups { QVariantList() };
    bool m_visible { false };
    bool m_groupEditMode { false };
    int m_editedItem { -1 };
    QSet<int> m_selectedGroups { QSet<int>() };
    QVariantList m_releaseNotUsedGroups { QVariantList() };
    QVariantList m_releaseUsedGroups { QVariantList() };
    QString m_releaseGroupsLink { "" };

public:
    explicit ReleaseCustomGroupsViewModel(QObject *parent = nullptr);

    QVariantList groups() const noexcept { return m_groups; }

    bool hasGroups() const noexcept { return !m_groups.isEmpty(); }

    QString releaseGroupsLink() const noexcept { return m_releaseGroupsLink; }

    bool visible() const noexcept { return m_visible; }
    void setVisible(bool visible) noexcept;

    bool releaseInFilter(int releaseId) const noexcept;

    bool groupEditMode() const noexcept { return m_groupEditMode; }

    QVariantList releaseNotUsedGroups() const noexcept { return m_releaseNotUsedGroups; }
    QVariantList releaseUsedGroups() const noexcept { return m_releaseUsedGroups; }

    Q_INVOKABLE void saveGroup(const QString& name) noexcept;
    Q_INVOKABLE void addReleaseIdToGroup(int index, int releaseId) noexcept;
    Q_INVOKABLE void deleteReleaseIdFromGroup(int index, int releaseId) noexcept;
    Q_INVOKABLE void startEditNewGroup() noexcept;
    Q_INVOKABLE void startEditGroup(int index) noexcept;
    Q_INVOKABLE void saveState() noexcept;
    Q_INVOKABLE void deleteGroup(int index) noexcept;
    Q_INVOKABLE void editGroup(int index, const QString& name) noexcept;
    Q_INVOKABLE void toggleSelectGroup(int index) noexcept;
    Q_INVOKABLE void addGroupToSelect(int index) noexcept;
    Q_INVOKABLE void cancelEdit() noexcept;
    Q_INVOKABLE void clearFilters() noexcept;
    Q_INVOKABLE void setupReleaseId(int releaseId) noexcept;

private:
    void refreshGroups();
    void loadGroups();
    void saveGroups();

signals:
    void groupsChanged();
    void visibleChanged();
    void hasGroupsChanged();
    void groupEditModeChanged();
    void releaseNotUsedGroupsChanged();
    void releaseUsedGroupsChanged();
    void releaseGroupsLinkChanged();

};

#endif // RELEASECUSTOMGROUPSVIEWMODEL_H
