namespace Aniliberty.Unfolded.Models.Releases
{

	public record DisplayFranchiseModel
	{

		public required string Title { get; init; } = "";

		public string Poster { get; init; } = "";

		public string AllTime { get; init; } = "";

		public int CountEpisodes { get; init; }

		public IEnumerable<DisplayFranchiseReleaseModel> Releases { get; set; } = Enumerable.Empty<DisplayFranchiseReleaseModel>();

	}

}
