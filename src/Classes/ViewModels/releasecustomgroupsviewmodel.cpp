#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include "releasecustomgroupsviewmodel.h"
#include "../../globalhelpers.h"

ReleaseCustomGroupsViewModel::ReleaseCustomGroupsViewModel(QObject *parent)
    : QObject{parent}
{
    createIfNotExistsFile(getCachePath(groupsCacheFileName), "[]");

    loadGroups();
}

void ReleaseCustomGroupsViewModel::setVisible(bool visible) noexcept
{
    if (m_visible == visible) return;

    m_visible = visible;
    emit visibleChanged();
}

bool ReleaseCustomGroupsViewModel::releaseInFilter(int releaseId) const noexcept
{
    if (m_selectedGroups.isEmpty()) return true;

    auto keys = m_groupNames.keys();
    foreach (auto groupKey, keys) {
        if (m_groupValues.contains(groupKey, releaseId)) return true;
    }

    return false;
}

void ReleaseCustomGroupsViewModel::saveGroup(const QString& name) noexcept
{
    if (m_editedItem == -1) {
        auto keys = m_groupNames.keys();
        auto index = std::max_element(keys.begin(), keys.end());
        if (index != keys.end()) {
            m_groupNames.insert(*index + 1, name);
        } else {
            m_groupNames.insert(0, name);
        }
    } else {
        m_groupNames[m_editedItem] = name;
    }

    m_groupEditMode = false;
    emit groupEditModeChanged();

    refreshGroups();
}

void ReleaseCustomGroupsViewModel::addReleaseIdToGroup(int index, int releaseId) noexcept
{
    if (m_groupValues.contains(index, releaseId)) return;

    m_groupValues.insert(index, releaseId);
}

void ReleaseCustomGroupsViewModel::startEditNewGroup() noexcept
{
    m_editedItem = -1;
    m_groupEditMode = true;
    emit groupEditModeChanged();
}

void ReleaseCustomGroupsViewModel::startEditGroup(int index) noexcept
{
    m_editedItem = index;
    m_groupEditMode = true;
    emit groupEditModeChanged();
}

void ReleaseCustomGroupsViewModel::saveState() noexcept
{
    saveGroups();
}

void ReleaseCustomGroupsViewModel::deleteGroup(int index) noexcept
{
    if (!m_groupNames.contains(index)) return;

    m_groupNames.remove(index);
    m_groupValues.remove(index);

    refreshGroups();
}

void ReleaseCustomGroupsViewModel::editGroup(int index, const QString& name) noexcept
{
    if (!m_groupNames.contains(index)) return;

    m_groupNames[index] = name;

    refreshGroups();
}

void ReleaseCustomGroupsViewModel::toggleSelectGroup(int index) noexcept
{
    if (m_selectedGroups.contains(index)) {
        m_selectedGroups.remove(index);
    } else {
        m_selectedGroups.insert(index);
    }

    refreshGroups();
}

void ReleaseCustomGroupsViewModel::cancelEdit() noexcept
{
    m_groupEditMode = false;
    emit groupEditModeChanged();
}

void ReleaseCustomGroupsViewModel::clearFilters() noexcept
{
    m_selectedGroups.clear();

    refreshGroups();
}

void ReleaseCustomGroupsViewModel::setupReleaseId(int releaseId) noexcept
{
    m_releaseNotUsedGroups.clear();
    m_releaseUsedGroups.clear();

    auto keys = m_groupNames.keys();
    foreach (auto groupKey, keys) {
        if (m_groupValues.contains(groupKey, releaseId)) {
            QVariantMap map;
            map["name"] = m_groupNames.value(groupKey);
            map["identifier"] = groupKey;
            m_releaseUsedGroups.append(map);
        } else {
            QVariantMap map;
            map["name"] = m_groupNames.value(groupKey);
            map["identifier"] = groupKey;
            m_releaseNotUsedGroups.append(map);
        }
    }

    emit releaseNotUsedGroupsChanged();
    emit releaseUsedGroupsChanged();
}

void ReleaseCustomGroupsViewModel::refreshGroups()
{
    if (!m_groups.isEmpty()) m_groups.clear();

    auto keys = m_groupNames.keys();
    foreach (auto key, keys) {
        QVariantMap value;
        value["index"] = key;
        value["groupName"] = m_groupNames[key];
        value["isSelected"] = m_selectedGroups.contains(key);
        m_groups.append(value);
    }

    emit groupsChanged();
    emit hasGroupsChanged();
}

void ReleaseCustomGroupsViewModel::loadGroups()
{
    QFile groupFile(getCachePath(groupsCacheFileName));
    if (!groupFile.open(QFile::ReadOnly | QFile::Text)) return;
    auto json = groupFile.readAll();
    groupFile.close();

    auto jsonDocument = QJsonDocument::fromJson(json);
    auto groups = jsonDocument.array();

    foreach (auto group, groups) {
        if (!group.isObject()) continue;

        auto itemObject = group.toObject();
        auto groupName = itemObject.value("name").toString();
        auto groupIndex = itemObject.value("index").toInt();
        auto groupType = itemObject.value("type").toString();
        m_groupNames.insert(groupIndex, groupName);

        if (groupType == "identifiers") {
            auto groupItemsIds = itemObject.value("items").toArray();

            foreach (auto item, groupItemsIds) {
                auto id = item.toInt();
                m_groupValues.insert(groupIndex, id);
            }
        }
    }

    refreshGroups();
}

void ReleaseCustomGroupsViewModel::saveGroups()
{
    QJsonArray array;

    auto keys = m_groupNames.keys();

    foreach (auto groupKey, keys) {
        QJsonObject item;
        item["name"] = m_groupNames[groupKey];
        item["index"] = groupKey;
        item["type"] = "identifiers";
        if (m_groupValues.contains(groupKey)) {            
            QJsonArray identifiers;
            auto values = m_groupValues.values(groupKey);
            foreach (auto value, values) {
                identifiers.append(value);
            }
            item["items"] = identifiers;
        }
        array.append(item);
    }

    QJsonDocument document(array);

    QFile groupFile(getCachePath(groupsCacheFileName));
    groupFile.open(QFile::WriteOnly | QFile::Text);
    groupFile.write(document.toJson());
    groupFile.close();
}

