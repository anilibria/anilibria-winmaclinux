#ifndef RELEASECUSTOMGROUPSVIEWMODEL_H
#define RELEASECUSTOMGROUPSVIEWMODEL_H

#include <QObject>
#include <QMap>
#include <QMultiMap>
#include <QVariantList>

class ReleaseCustomGroupsViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList groups READ groups NOTIFY groupsChanged)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(bool hasGroups READ hasGroups NOTIFY hasGroupsChanged)
    Q_PROPERTY(bool addGroupEditMode READ addGroupEditMode NOTIFY addGroupEditModeChanged)

private:
    const QString groupsCacheFileName { "groups.cache" };
    QMap<int, QString> m_groupNames { QMap<int, QString>() };
    QMultiMap<int, int> m_groupValues { QMultiMap<int, int>() };
    QVariantList m_groups { QVariantList() };
    bool m_visible { false };
    bool m_addGroupEditMode { false };

public:
    explicit ReleaseCustomGroupsViewModel(QObject *parent = nullptr);

    QVariantList groups() const noexcept { return m_groups; }

    bool hasGroups() const noexcept { return !m_groups.isEmpty(); }

    bool visible() const noexcept { return m_visible; }
    void setVisible(bool visible) noexcept;

    bool addGroupEditMode() const noexcept { return m_addGroupEditMode; }

    Q_INVOKABLE void addGroup(const QString& name) noexcept;
    Q_INVOKABLE void addReleaseIdToGroup(int index, int releaseId) noexcept;
    Q_INVOKABLE void startEditNewGroup() noexcept;
    Q_INVOKABLE void saveState() noexcept;

private:
    void refreshGroups();
    void loadGroups();
    void saveGroups();

signals:
    void groupsChanged();
    void visibleChanged();
    void hasGroupsChanged();
    void addGroupEditModeChanged();

};

#endif // RELEASECUSTOMGROUPSVIEWMODEL_H
