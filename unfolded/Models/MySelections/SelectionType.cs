namespace Aniliberty.Unfolded.Models.MySelections
{

	public enum SelectionType
	{

		UpdateByFavorite = 1, // Обновление по избранному

		AbandonedView = 2, // Брошенный просмотр

		WillBeView = 3, // Буду смотреть

		WhatViewFurther = 4, // Что посмотреть дальше

		UpdateForLastSession = 5, // Обновление с последнего посещения

		LastUpdate = 6, // Последние обновления

		CurrentSeason = 7, // Текущий сезон

		ActualInCurrentSeason = 8, // Актуально в текущем сезоне

		RecomendationForVoices = 9, // Рекомендации по войсерам

		RecomendationForGenres = 10, // Рекомендации по жанрам

		UserSearchFilter = 11, // Пользовательский фильтр

		UserGroup = 12, // Пользовательская группа

	};

}