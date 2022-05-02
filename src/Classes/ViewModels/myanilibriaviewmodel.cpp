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
    m_pathToCacheFile = getCachePath(m_cacheFileName);
    createIfNotExistsFile(m_pathToCacheFile, "[]");

    connect(m_myList.get(), &MyAnilibriaListModel::needSaveSections, this, &MyAnilibriaViewModel::saveSections);
}

void MyAnilibriaViewModel::setReleasesViewModel(const ReleasesViewModel *viewModel) noexcept
{
    if (m_releasesViewModel != nullptr) return; // not allowed setup viewmodel twice and more

    m_releasesViewModel = const_cast<ReleasesViewModel *>(viewModel);
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

    if (sectionArray.isEmpty()) return;

    QSet<QString> m_sections;
    foreach (auto item, sectionArray) {
        m_sections.insert(item.toString());
    }
    m_myList->fillSections(m_sections);
}

void MyAnilibriaViewModel::saveSections()
{
    auto sections = m_myList->getSections();
    QJsonArray array;
    foreach (auto section, sections) {
        array.append(section);
    }

    QJsonDocument jsonDocument(array);

    QFile cacheFile(m_pathToCacheFile);
    if (!cacheFile.open(QFile::WriteOnly | QFile::Text)) return;

    cacheFile.write(jsonDocument.toJson());
    cacheFile.close();
}
