#include "filterDictionariesViewModel.h"

FilterDictionariesViewModel::FilterDictionariesViewModel(QObject *parent)
    : QObject{parent}
{

}

void FilterDictionariesViewModel::setReleases(const ReleasesViewModel *releases) noexcept
{
    if (m_releases == releases) return;

    m_releases = const_cast<ReleasesViewModel *>(releases);

    fillDictinaries();
}

void FilterDictionariesViewModel::refreshDictionaries()
{
    m_years.clear();
    m_genres.clear();
    m_statuses.clear();
    m_voices.clear();
    m_seasons.clear();

    fillDictinaries();
}

void FilterDictionariesViewModel::fillDictinaries()
{
    QSet<QString> years;
    QSet<QString> genres;
    QSet<QString> statuses;
    QSet<QString> voices;
    QSet<QString> seasons;

    auto func = [&](FullReleaseModel* model) {
        if (!model->year().isEmpty()) years.insert(model->year());
        if (!model->season().isEmpty()) seasons.insert(model->season());
        if (!model->status().isEmpty()) statuses.insert(model->status());
        if (!model->genres().isEmpty()) {
            auto items = model->genres().split(",");
            foreach (auto item, items) {
                auto genre = item.trimmed().toLower();
                if (!genre.isEmpty()) genres.insert(genre);
            }
        }
        if (!model->voicers().isEmpty()) {
            auto items = model->voicers().split(",");
            foreach (auto item, items) {
                auto voice = item.trimmed().toLower();
                if (!voice.isEmpty()) voices.insert(voice);
            }
        }
    };

    m_releases->iterateOnReleases(func);

    m_genres.append(genres.values());
    m_years.append(years.values());
    std::sort(m_years.begin(), m_years.end(), [](QString left, QString right) { return left > right; });
    m_statuses.append(statuses.values());
    m_voices.append(voices.values());
    std::sort(m_voices.begin(), m_voices.end(), [](QString left, QString right) { return left < right; });
    m_seasons.append(seasons.values());
    std::sort(m_seasons.begin(), m_seasons.end(), [](QString left, QString right) { return left < right; });
}

