#ifndef MYANILIBRIAVIEWMODEL_H
#define MYANILIBRIAVIEWMODEL_H

#include <QObject>
#include "./releasesviewmodel.h"

class MyAnilibriaViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ReleasesViewModel* releasesViewModel READ releasesViewModel WRITE setReleasesViewModel NOTIFY releasesViewModelChanged)
    Q_PROPERTY(QString genres READ genres NOTIFY genresChanged)

private:
    ReleasesViewModel* m_releasesViewModel { nullptr };

public:
    explicit MyAnilibriaViewModel(QObject *parent = nullptr);

    ReleasesViewModel* releasesViewModel() const noexcept { return m_releasesViewModel; }
    void setReleasesViewModel(const ReleasesViewModel* viewModel) noexcept;

    QString genres() const noexcept;

signals:
    void releasesViewModelChanged();
    void genresChanged();

};

#endif // MYANILIBRIAVIEWMODEL_H
