using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.Releases
{

	public record ReleaseSeriesSaveModel
	{

		[JsonPropertyName("count")]
		public int CountReleases { get; init; } = 0;

		[JsonPropertyName("pos")]
		public string Poster { get; init; } = "";

		[JsonPropertyName("rels")]
		public IEnumerable<ReleaseSeriesReleaseSaveModel> Releases { get; set; } = Enumerable.Empty<ReleaseSeriesReleaseSaveModel>();

		[JsonPropertyName("tit")]
		public string Title { get; init; } = "";

		[JsonPropertyName("sec")]
		public long CountSeconds { get; set; }

		[JsonPropertyName("eps")]
		public int CountEpisodes { get; set; }

		[JsonPropertyName("rat")]
		public decimal Rating { get; set; }

	}

	public record ReleaseSeriesReleaseSaveModel
	{
		[JsonPropertyName("id")]
		public int Id { get; set; }

		[JsonPropertyName("ti")]
		public string Title { get; init; } = "";

		[JsonPropertyName("po")]
		public string Poster { get; init; } = "";

	}

}
