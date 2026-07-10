using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

	internal class FranchiseModel
	{

		[JsonPropertyName("id")]
		public string Id { get; set; } = "";

		[JsonPropertyName("name")]
		public string Name { get; set; } = "";

		[JsonPropertyName("name_english")]
		public string NameEnglish { get; set; } = "";

		[JsonPropertyName("rating")]
		public decimal? Rating { get; set; } = null;

		[JsonPropertyName("last_year")]
		public int? LastYear { get; set; }

		[JsonPropertyName("first_year")]
		public int? FirstYear { get; set; }

		[JsonPropertyName("total_releases")]
		public int? TotalReleases { get; set; }

		[JsonPropertyName("total_episodes")]
		public int? TotalEpisodes { get; set; }

		[JsonPropertyName("total_duration")]
		public string TotalDuration { get; set; } = "";

		[JsonPropertyName("total_duration_in_seconds")]
		public int? TotalDurationInSeconds { get; set; }

		[JsonPropertyName("image")]
		public FranchiseImageModel Image { get; set; } = new FranchiseImageModel();

	}

}
