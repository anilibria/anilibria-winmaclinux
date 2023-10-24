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

void ReleaseCustomGroupsViewModel::addGroup(const QString& name) noexcept
{
    auto keys = m_groupNames.keys();
    auto index = std::max_element(keys.begin(), keys.end());
    if (index != keys.end()) {
        m_groupNames.insert(*index + 1, name);
    } else {
        m_groupNames.insert(0, name);
    }

    m_addGroupEditMode = false;
    emit addGroupEditModeChanged();

    refreshGroups();
}

void ReleaseCustomGroupsViewModel::addReleaseIdToGroup(int index, int releaseId) noexcept
{
    if (m_groupValues.contains(index, releaseId)) return;

    m_groupValues.insert(index, releaseId);
}

void ReleaseCustomGroupsViewModel::startEditNewGroup() noexcept
{
    m_addGroupEditMode = true;
    emit addGroupEditModeChanged();
}

void ReleaseCustomGroupsViewModel::saveState() noexcept
{
    saveGroups();
}

void ReleaseCustomGroupsViewModel::refreshGroups()
{
    if (!m_groups.isEmpty()) m_groups.clear();

    auto keys = m_groupNames.keys();
    foreach (auto key, keys) {
        QVariantMap value;
        value["index"] = key;
        value["groupName"] = m_groupNames[key];
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
    }

    QJsonDocument document(array);

    QFile groupFile(getCachePath(groupsCacheFileName));
    groupFile.open(QFile::WriteOnly | QFile::Text);
    groupFile.write(document.toJson());
    groupFile.close();
}

