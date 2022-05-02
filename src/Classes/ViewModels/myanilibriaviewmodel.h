#ifndef MYANILIBRIAVIEWMODEL_H
#define MYANILIBRIAVIEWMODEL_H

#include <QObject>
#include "./releasesviewmodel.h"
#include "../ListModels/myanilibrialistmodel.h"

class MyAnilibriaViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ReleasesViewModel* releasesViewModel READ releasesViewModel WRITE setReleasesViewModel NOTIFY releasesViewModelChanged)
    Q_PROPERTY(MyAnilibriaListModel* myList READ myList NOTIFY myListChanged)
    Q_PROPERTY(QString genres READ genres NOTIFY genresChanged)
    Q_PROPERTY(QString voices READ voices NOTIFY voicesChanged)

private:
    QString m_cacheFileName { "myanilibrialist.cache" };
    QString m_pathToCacheFile { "" };
    ReleasesViewModel* m_releasesViewModel { nullptr };
    QScopedPointer<MyAnilibriaListModel> m_myList { new MyAnilibriaListModel() };

public:
    explicit MyAnilibriaViewModel(QObject *parent = nullptr);

    ReleasesViewModel* releasesViewModel() const noexcept { return m_releasesViewModel; }
    void setReleasesViewModel(const ReleasesViewModel* viewModel) noexcept;

    MyAnilibriaListModel* myList() const noexcept { return m_myList.get(); }

    QString genres() const noexcept;
    QString voices() const noexcept;

private:
    void readFromCache() noexcept;

private slots:
    void saveSections();

signals:
    void releasesViewModelChanged();
    void genresChanged();
    void voicesChanged();
    void myListChanged();

};

#endif // MYANILIBRIAVIEWMODEL_H
