using Aniliberty.Unfolded.Models.Releases;

namespace Aniliberty.Unfolded.Models.MySelections
{

	public record SelectionModel
	{

		public SelectionType Type { get; init; }

		public int? MaximumReleases { get; init; }

		public ReleasesListFiltersModelSortingField? SortingField { get; init; }

		public bool? SortingDescending { get; init; }

		public int? HowMuchDaysExpire { get; set; }

		public string Name { get; set; } = "";

	}

}
