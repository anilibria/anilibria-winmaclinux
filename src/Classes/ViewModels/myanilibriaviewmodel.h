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

#ifndef MYANILIBRIAVIEWMODEL_H
#define MYANILIBRIAVIEWMODEL_H

#include <QObject>
#include "./releasesviewmodel.h"
#include "../ListModels/myanilibrialistmodel.h"
#include "../ListModels/allmyanilibrialistmodel.h"
#include "../ListModels/releasesimplelistmodel.h"

class MyAnilibriaViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ReleasesViewModel* releasesViewModel READ releasesViewModel WRITE setReleasesViewModel NOTIFY releasesViewModelChanged)
    Q_PROPERTY(MyAnilibriaListModel* myList READ myList NOTIFY myListChanged)
    Q_PROPERTY(AllMyAnilibriaListModel* allSections READ allSections NOTIFY allSectionsChanged)
    Q_PROPERTY(QString genres READ genres NOTIFY genresChanged)
    Q_PROPERTY(QString voices READ voices NOTIFY voicesChanged)
    Q_PROPERTY(QString hoveredDescription READ hoveredDescription WRITE setHoveredDescription NOTIFY hoveredDescriptionChanged)
    Q_PROPERTY(int restoreScroll READ restoreScroll WRITE setRestoreScroll NOTIFY restoreScrollChanged)
    Q_PROPERTY(ReleaseSimpleListModel* zeroSectionModel READ zeroSectionModel NOTIFY zeroSectionModelChanged)

private:
    QString m_cacheFileName { "myanilibrialist.cache" };
    QSharedPointer<QMap<int, QString>> m_fullSectionsOrders { new QMap<int, QString>() };
    QSharedPointer<QMap<QString, QString>> m_sectionTitles { new QMap<QString, QString>() };
    QSharedPointer<QMap<int, QString>> m_sectionOrders { new QMap<int, QString>() };
    QString m_pathToCacheFile { "" };
    ReleasesViewModel* m_releasesViewModel { nullptr };
    QScopedPointer<MyAnilibriaListModel> m_myList { new MyAnilibriaListModel() };
    QScopedPointer<AllMyAnilibriaListModel> m_allList { new AllMyAnilibriaListModel() };
    QSharedPointer<QSet<QString>> m_selectedSections { new QSet<QString>() };
    QMap<QString, ReleaseSimpleListModel*> m_sectionModels { QMap<QString, ReleaseSimpleListModel*>() };
    QString m_hoveredDescription { "" };
    int m_restoreScroll { 0 };
    ReleaseSimpleListModel* m_zeroSectionModel { new ReleaseSimpleListModel(this) };

public:
    explicit MyAnilibriaViewModel(QObject *parent = nullptr);

    ReleasesViewModel* releasesViewModel() const noexcept { return m_releasesViewModel; }
    void setReleasesViewModel(const ReleasesViewModel* viewModel) noexcept;

    MyAnilibriaListModel* myList() const noexcept { return m_myList.get(); }

    AllMyAnilibriaListModel* allSections() const noexcept { return m_allList.get(); }

    QString hoveredDescription() const noexcept { return m_hoveredDescription; }
    void setHoveredDescription(const QString& hoveredDescription) noexcept;

    QString genres() const noexcept;
    QString voices() const noexcept;

    int restoreScroll() const noexcept { return m_restoreScroll; }
    void setRestoreScroll(int restoreScroll) noexcept;

    ReleaseSimpleListModel* zeroSectionModel() const noexcept { return m_zeroSectionModel; }

    Q_INVOKABLE void selectSection(const QString& section) noexcept;
    Q_INVOKABLE void deselectSection(const QString& section) noexcept;
    Q_INVOKABLE void saveSectionsToFile();
    Q_INVOKABLE void moveSection(const int direction, const int index) noexcept;
    Q_INVOKABLE ReleaseSimpleListModel* getSectionModel(const QString& section) noexcept;
    Q_INVOKABLE void refreshAllSectionsModels() noexcept;

private:
    void readFromCache() noexcept;

private slots:
    void saveSections();

signals:
    void releasesViewModelChanged();
    void genresChanged();
    void voicesChanged();
    void myListChanged();
    void allSectionsChanged();
    void hoveredDescriptionChanged();
    void restoreScrollChanged();
    void needRestoreScroll();
    void zeroSectionModelChanged();

};

#endif // MYANILIBRIAVIEWMODEL_H
