/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2021 Roman Vladimirov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "myanilibrialistmodel.h"
#include "../../globalconstants.h"

MyAnilibriaListModel::MyAnilibriaListModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

void MyAnilibriaListModel::setup(QSharedPointer<QMap<QString, QString>> sectionTitles, QSharedPointer<QMap<int, QString>> sectionOrders, QSharedPointer<QSet<QString>> selectedSections)
{
    m_sectionTitles = sectionTitles;
    m_sectionOrders = sectionOrders;
    m_selectedSections = selectedSections;
}

int MyAnilibriaListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_selectedSections->size();
}

QVariant MyAnilibriaListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto section = m_sectionOrders->value(index.row());
    auto title = m_sectionTitles->value(section);
    auto currentIndex = index.row();

    switch (role) {
        case SectionIdRole: {
            return QVariant(section);
        }
        case SectionTitleRole: {
            return QVariant(title);
        }
        case ComponentIdRole: {
            if (section == StatisticsSectionId) {
                return QVariant(StatisticsSectionComponent);
            } else {
                return QVariant(SimpleListSectionComponent);
            }
        }
        case HeaderVisibleRole: {
            return QVariant(getHideSection(section));
        }
        case CurrentIndexRole: {
            return QVariant(currentIndex);
        }
        case IsLastRole: {
            return QVariant(currentIndex == m_sectionOrders->count() - 1);
        }
        case IsFirstRole: {
            return QVariant(currentIndex == 0);
        }
    }

    return QVariant();
}

QHash<int, QByteArray> MyAnilibriaListModel::roleNames() const
{
    return {
        {
            SectionIdRole,
            "sectionId"
        },
        {
            SectionTitleRole,
            "title"
        },
        {
            ComponentIdRole,
            "componentId"
        },
        {
            HeaderVisibleRole,
            "needHideSection"
        },
        {
            CurrentIndexRole,
            "currentIndex"
        },
        {
            IsLastRole,
            "lastSection"
        },
        {
            IsFirstRole,
            "firstSection"
        }
    };
}

void MyAnilibriaListModel::setUserConfiguration(const UserConfigurationViewModel *userConfiguration) noexcept
{
    if (m_userConfiguration == userConfiguration) return;

    m_userConfiguration = const_cast<UserConfigurationViewModel *>(userConfiguration);
    emit userConfigurationChanged();
}

void MyAnilibriaListModel::setSectionHideMark(const QString &section, const bool notVisible) noexcept
{
    if (section == StatisticsSectionId) m_userConfiguration->setHideStatistics(notVisible);
    if (section == NewInFavoritesSectionId) m_userConfiguration->setHideUpdatesByFavorites(notVisible);
    if (section == NewFromStartSectionId) m_userConfiguration->setHideUpdatesFromStart(notVisible);
    if (section == LastTwoDaysSectionId) m_userConfiguration->setHideUpdatesLastTwoDays(notVisible);
    if (section == AbandonedSeensSectionId) m_userConfiguration->setHideAbandonedSeens(notVisible);
}

void MyAnilibriaListModel::refresh() noexcept
{
    beginResetModel();
    endResetModel();
}

void MyAnilibriaListModel::toggleSectionHideMark(const int elementIndex)
{
    auto section = m_sectionOrders->value(elementIndex);
    if (section == StatisticsSectionId) {
        auto current = m_userConfiguration->hideStatistics();
        m_userConfiguration->setHideStatistics(!current);
    }
    if (section == NewInFavoritesSectionId) {
        auto current = m_userConfiguration->hideUpdatesByFavorites();
        m_userConfiguration->setHideUpdatesByFavorites(!current);
    }
    if (section == NewFromStartSectionId) {
        auto current = m_userConfiguration->hideUpdatesFromStart();
        m_userConfiguration->setHideUpdatesFromStart(!current);
    }
    if (section == LastTwoDaysSectionId) {
        auto current = m_userConfiguration->hideUpdatesLastTwoDays();
        m_userConfiguration->setHideUpdatesLastTwoDays(!current);
    }
    if (section == AbandonedSeensSectionId) {
        auto current = m_userConfiguration->hideAbandonedSeens();
        m_userConfiguration->setHideAbandonedSeens(!current);
    }

    emit dataChanged(index(elementIndex,0), index(elementIndex,0));
}

void MyAnilibriaListModel::setNotVisibleAllMarks(const bool visible)
{
    beginResetModel();

    foreach (auto section, *m_selectedSections) {
        setSectionHideMark(section, visible);
    }

    endResetModel();
}

bool MyAnilibriaListModel::getHideSection(const QString &section) const noexcept
{
    if (section == StatisticsSectionId) {
        return m_userConfiguration->hideStatistics();
    }
    if (section == NewInFavoritesSectionId) {
        return m_userConfiguration->hideUpdatesByFavorites();
    }
    if (section == NewFromStartSectionId) {
        return m_userConfiguration->hideUpdatesFromStart();
    }
    if (section == LastTwoDaysSectionId) {
        return m_userConfiguration->hideUpdatesLastTwoDays();
    }
    if (section == AbandonedSeensSectionId) {
        return m_userConfiguration->hideAbandonedSeens();
    }

    return false;
}
