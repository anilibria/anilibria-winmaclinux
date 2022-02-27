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
