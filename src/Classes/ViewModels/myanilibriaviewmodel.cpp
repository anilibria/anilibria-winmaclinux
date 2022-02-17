#include "myanilibriaviewmodel.h"

MyAnilibriaViewModel::MyAnilibriaViewModel(QObject *parent)
    : QObject{parent}
{

}

void MyAnilibriaViewModel::setReleasesViewModel(const ReleasesViewModel *viewModel) noexcept
{
    if (m_releasesViewModel != nullptr) return; // not allowed setup viewmodel twice and more

    m_releasesViewModel = const_cast<ReleasesViewModel *>(viewModel);
}

QString MyAnilibriaViewModel::genres() const noexcept
{
    return m_releasesViewModel->getMostPopularGenres().join(", ");
}
