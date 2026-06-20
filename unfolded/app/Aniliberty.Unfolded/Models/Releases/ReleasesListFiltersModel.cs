namespace Aniliberty.Unfolded.Models.Releases
{

	internal record ReleasesListFiltersModel
	{

		public string? Filter { get; init; } = "";

		public string? Type { get; init; } = "";

		public string? Description { get; init; } = "";

		public IEnumerable<string>? Voices { get; init; } = [];

		public bool? VoicesOr { get; set; }

		public IEnumerable<string>? Genres { get; init; } = [];

		public bool? GenresOr { get; set; }

		public IEnumerable<int>? Years { get; init; } = [];

		public bool? YearsOr { get; set; }

		public IEnumerable<string>? Seasons { get; init; } = [];

		public bool? SeasonsOr { get; set; }

		public IEnumerable<string>? Statuses { get; init; } = [];

		public bool? StatusesOr { get; set; }

		public IEnumerable<string>? ScheduleDays { get; init; } = [];

		public bool? ScheduleDaysOr { get; set; }

		public bool? InFavorites { get; init; } = null;

		public bool? PartOfReleases { get; init; }

		public ReleasesListFiltersModelSeenStatus? SeenStatus { get; init; }

		public ReleasesListFiltersModelSortingField? SortingField { get; init; }

		public bool? SortingDescending { get; init; }

		public ReleasesListFiltersSection Section { get; init; }

	}

	public enum ReleasesListFiltersSection
	{

		// Base items

		All = 0,

		Favorites = 1,

		Notifications = 2,

		Schedule = 3,

		Alphabet = 4,

		Groups = 5,

		History = 6,

		Seens = 7,

		// Sub items

		OpenHistory = 40,

		SeenHistory = 41,

		Seen = 42,

		SeenNow = 43,

		NotSeen = 44,

		Hided = 45,

		SeenToEnd = 46,

		SeenNotToEnd = 47,

		Films = 48,

		MostPopular = 49,

		PartOfRelease = 50,

		Completed = 51,

		PopulInCurrentYear = 52,

		InCinemaHall = 53,

		CurrentSeason = 54,

		NotCurrentSeason = 55,

	};

	public enum ReleasesListFiltersModelSortingField
	{

		DateUpdate = 0,

		ScheduleDay = 1,

		Name = 2,

		Year = 3,

		Rating = 4,

		Status = 5,

		OriginalName = 6,

		OpenHistory = 7,

		SeenHistory = 8,

		Season = 9,

		Favorite = 10,

		SeenMark = 11,

		ReleaseSeries = 12,

	};

	public enum ReleasesListFiltersModelSeenStatus
	{

		NoSelected = 0,

		Seen = 1,

		InSeen = 2,

		NotSeen = 3

	};

}
