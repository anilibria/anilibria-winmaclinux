namespace Aniliberty.Unfolded.Models.Releases
{

	internal record ReleasesListFiltersModel
	{

		public string Filter { get; init; } = "";

		public string Type { get; init; } = "";

		public string Description { get; init; } = "";

		public IEnumerable<string> Voices { get; init; } = [];

		public bool VoicesOr { get; set; }

		public IEnumerable<string> Genres { get; init; } = [];

		public bool GenresOr { get; set; }

		public IEnumerable<int> Years { get; init; } = [];

		public bool YearsOr { get; set; }

		public IEnumerable<string> Seasons { get; init; } = [];

		public bool SeasonsOr { get; set; }

		public IEnumerable<string> Statuses { get; init; } = [];

		public bool StatusesOr { get; set; }

		public IEnumerable<string> ScheduleDays { get; init; } = [];

		public bool ScheduleDaysOr { get; set; }

		public bool? InFavorites { get; init; } = null;

		public bool PartOfReleases { get; init; }

		public ReleasesListFiltersModelSeenStatus SeenStatus { get; init; }

		public ReleasesListFiltersModelSortingField SortingField { get; init; }

		public bool SortingDescending { get; init; }

		public ReleasesListFiltersSection Section { get; init; }

		public int Page { get; init; }

	}

	public enum ReleasesListFiltersSection
	{

		All = 0,

		Favorites = 1,

		Notifications = 2,

		Schedule = 3,

		OpenHistory = 4,

		SeenHistory = 5,

		Alphabet = 6,

		Groups = 7,

		Seens = 8,

		SeenNow = 9,

		NotSeen = 10,

		Hided = 11,

		SeenToEnd = 12,

		SeenNotToEnd = 13,

		Films = 50,

		MostPopular = 51,

		PartOfRelease = 52,

		Completed = 53,

		PopulInCurrentYear = 54,

		InCinemaHall = 55,

		CurrentSeason = 56,

		NotCurrentSeason = 57,

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
