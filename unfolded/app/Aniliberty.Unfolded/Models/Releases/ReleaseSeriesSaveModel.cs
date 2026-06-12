using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.Releases
{

	public record ReleaseSeriesSaveModel
	{

		[JsonPropertyName("count")]
		public int CountReleases { get; init; } = 0;

		[JsonPropertyName("pos")]
		public string Poster { get; init; } = "";

		[JsonPropertyName("poss")]
		public IEnumerable<string> Posters { get; init; } = [];

		[JsonPropertyName("relids")]
		public IEnumerable<int> ReleasesIds { get; init; } = [];

		[JsonPropertyName("tits")]
		public IEnumerable<string> Titles { get; init; } = [];

		[JsonPropertyName("tit")]
		public string Title { get; init; } = "";

		[JsonPropertyName("sec")]
		public long CountSeconds { get; set; }

		[JsonPropertyName("eps")]
		public int CountEpisodes { get; set; }

		[JsonPropertyName("rat")]
		public decimal Rating { get; set; }

	}

}
