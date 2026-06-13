namespace Aniliberty.Unfolded.Models.Releases
{

	public record ReleaseSaveModel
	{

		public string Announce { get; init; } = "";

		public string Code { get; init; } = "";

		public int CountTorrents { get; init; }

		public int CountVideos { get; init; }

		public string Description { get; init; } = "";

		public IEnumerable<string> Genres { get; init; } = Enumerable.Empty<string>();

		public int Id { get; init; }

		public string OriginalName { get; init; } = "";

		public string Poster { get; init; } = "";

		public int Rating { get; init; }

		public string Season { get; init; } = ""; // need to remake on int

		public int Series { get; init; }

		public string Status { get; init; } = ""; // need to remake on int

		public long Timestamp { get; init; }

		public string Title { get; init; } = "";

		public string Type { get; init; } = "";  // need to remake on int

		public IEnumerable<string> Voices { get; init; } = Enumerable.Empty<string>();

		public IEnumerable<string> Team { get; init; } = Enumerable.Empty<string>();

		public int Year { get; init; }

		public bool IsOngoing { get; init; }

		public string AgeRating { get; init; } = "";

	}

}
