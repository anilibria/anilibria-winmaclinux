using Aniliberty.Unfolded.Models.Releases;

namespace Aniliberty.Unfolded.Models.MySelections
{

	public record SelectionModel
	{

		public string Id { get; set; } = "";

		public SelectionType Type { get; init; }

		public int? MaximumReleases { get; init; }

		public ReleasesListFiltersModelSortingField? SortingField { get; init; }

		public bool? SortingDescending { get; init; }

		public int? HowMuchDaysExpire { get; set; }

		public string Name { get; set; } = "";

		public ReleasesListFiltersSection? Section { get; init; }

		public ReleasesListFiltersSubSection? SubSection { get; init; }

		public string? Filter { get; init; } = "";

	}

}
