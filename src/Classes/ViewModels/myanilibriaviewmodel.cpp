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

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSet>
#include "myanilibriaviewmodel.h"
#include "../../globalhelpers.h"

MyAnilibriaViewModel::MyAnilibriaViewModel(QObject *parent)
    : QObject{parent}
{
    m_fullSectionsOrders->insert(0, StatisticsSectionId);
    m_fullSectionsOrders->insert(1, NewInFavoritesSectionId);
    m_fullSectionsOrders->insert(2, NewFromStartSectionId);
    m_fullSectionsOrders->insert(3, LastTwoDaysSectionId);
    m_fullSectionsOrders->insert(4, AbandonedSeensSectionId);
    m_fullSectionsOrders->insert(5, RecommendedByGenresSectionId);
    m_fullSectionsOrders->insert(6, WillWatchSectionId);

    m_sectionTitles->insert(StatisticsSectionId, "Статистика");
    m_sectionTitles->insert(NewInFavoritesSectionId, "Обновления по избранному");
    m_sectionTitles->insert(NewFromStartSectionId, "Обновления с последнего посещения");
    m_sectionTitles->insert(LastTwoDaysSectionId, "Последние обновления");
    m_sectionTitles->insert(AbandonedSeensSectionId, "Брошенный просмотр");
    m_sectionTitles->insert(RecommendedByGenresSectionId, "Рекомендации по жанру");
    m_sectionTitles->insert(WillWatchSectionId, "Буду смотреть");

    m_myList->setup(m_sectionTitles, m_sectionOrders , m_selectedSections);
    m_allList->setup(m_fullSectionsOrders, m_sectionTitles, m_selectedSections);

    m_pathToCacheFile = getCachePath(m_cacheFileName);
    createIfNotExistsFile(m_pathToCacheFile, "[]");
    readFromCache();
}

void MyAnilibriaViewModel::setReleasesViewModel(const ReleasesViewModel *viewModel) noexcept
{
    if (m_releasesViewModel != nullptr) return; // not allowed setup viewmodel twice and more

    m_releasesViewModel = const_cast<ReleasesViewModel *>(viewModel);
}

void MyAnilibriaViewModel::setHoveredDescription(const QString &hoveredDescription) noexcept
{
    if (m_hoveredDescription == hoveredDescription) return;

    m_hoveredDescription = hoveredDescription;
    emit hoveredDescriptionChanged();
}

QString MyAnilibriaViewModel::genres() const noexcept
{
    auto genres = m_releasesViewModel->getMostPopularGenres();
    if (genres.isEmpty()) return "Нет данных";

    return genres.join(", ");
}

QString MyAnilibriaViewModel::voices() const noexcept
{
    auto voices = m_releasesViewModel->getMostPopularVoices();
    if (voices.isEmpty()) return "Нет данных";

    return voices.join(", ");
}

void MyAnilibriaViewModel::selectSection(const QString &section) noexcept
{
    if (m_selectedSections->contains(section)) return;

    m_selectedSections->insert(section);
    m_sectionOrders->insert(m_sectionOrders->count(), section);

    m_allList->refreshData();
    m_myList->refresh();
}

void MyAnilibriaViewModel::deselectSection(const QString &section) noexcept
{
    if (!m_selectedSections->contains(section)) return;

    m_selectedSections->remove(section);

    auto index = m_sectionOrders->values().indexOf(section);
    m_sectionOrders->remove(index);

    auto changedIndex = index + 1;

    while (true) {
        if (!m_sectionOrders->contains(changedIndex)) break;

        auto value = m_sectionOrders->value(changedIndex);
        m_sectionOrders->remove(changedIndex);
        m_sectionOrders->insert(changedIndex - 1, value);

        changedIndex++;
    }

    m_allList->refreshData();
    m_myList->refresh();
}

void MyAnilibriaViewModel::saveSectionsToFile()
{
    saveSections();
}

void MyAnilibriaViewModel::moveSection(const int direction, const int index) noexcept
{
    if (direction == MovingUp && index == 0) return;
    if (direction == MovingDown && index >= m_sectionOrders->count() - 1) return;

    if (direction == MovingUp) {
        auto previous = m_sectionOrders->value(index - 1);
        auto current = m_sectionOrders->value(index);
        (*m_sectionOrders)[index] = previous;
        (*m_sectionOrders)[index - 1] = current;
    }
    if (direction == MovingDown) {
        auto next = m_sectionOrders->value(index + 1);
        auto current = m_sectionOrders->value(index);
        (*m_sectionOrders)[index] = next;
        (*m_sectionOrders)[index + 1] = current;
    }

    m_myList->refresh();
}

void MyAnilibriaViewModel::readFromCache() noexcept
{
    QFile cacheFile(m_pathToCacheFile);
    if (!cacheFile.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }
    auto data = cacheFile.readAll();
    cacheFile.close();

    auto jsonDocument = QJsonDocument::fromJson(data);
    auto sectionArray = jsonDocument.array();

    if (sectionArray.isEmpty()) {
        auto currentIndex = -1;
        // if array is empty mark all sections as selected
        while (currentIndex < m_fullSectionsOrders->count()) {
            currentIndex++;
            auto section = m_fullSectionsOrders->value(currentIndex);
            m_sectionOrders->insert(currentIndex, section);
            m_selectedSections->insert(section);
        }
        return;
    }

    auto currentIndex = -1;
    foreach (auto item, sectionArray) {
        auto section = item.toString();
        if (section.isEmpty()) continue;

        currentIndex++;
        m_sectionOrders->insert(currentIndex, section);
        m_selectedSections->insert(section);
    }
    m_myList->refresh();
}

void MyAnilibriaViewModel::saveSections()
{
    QJsonArray array;
    for (auto i = 0; i < m_sectionOrders->count(); i++) {
         array.append(m_sectionOrders->value(i));
    }

    QJsonDocument jsonDocument(array);

    QFile cacheFile(m_pathToCacheFile);
    if (!cacheFile.open(QFile::WriteOnly | QFile::Text)) return;

    cacheFile.write(jsonDocument.toJson());
    cacheFile.close();
}
