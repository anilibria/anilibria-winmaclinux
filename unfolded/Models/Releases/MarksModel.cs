namespace Aniliberty.Unfolded.Models.Releases
{

	internal record MarksModel
	{

		public IEnumerable<int> Favorites { get; init; } = Enumerable.Empty<int>();

		public IEnumerable<int> FullSeenReleases { get; init; } = Enumerable.Empty<int>();

		public Dictionary<int, int> SeenSeries { get; init; } = new Dictionary<int, int>();

	}

}
