using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

	internal record ReleaseEpisodeModel
	{

		[JsonPropertyName("id")]
		public string Id { get; set; } = "";

		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull), JsonPropertyName("name")]
		public string Name { get; set; } = "";

		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull), JsonPropertyName("name_english")]
		public string NameEnglish { get; set; } = "";

		[JsonPropertyName("ordinal")]
		public decimal Ordinal { get; set; }

		[JsonPropertyName("sort_order")]
		public int SortOrder { get; set; }

		[JsonPropertyName("hls_480")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
		public string Hls480 { get; set; } = "";

		[JsonPropertyName("hls_720")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
		public string Hls720 { get; set; } = "";

		[JsonPropertyName("hls_1080")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
		public string Hls1080 { get; set; } = "";

		[JsonPropertyName("duration")]
		public long Duration { get; set; }

		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull), JsonPropertyName("rutube_id")]
		public string RutubeId { get; set; } = "";

		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull), JsonPropertyName("youtube_id")]
		public string YoutubeId { get; set; } = "";

		[JsonPropertyName("updated_at")]
		public string UpdatedAt { get; set; } = "";

		[JsonPropertyName("opening")]
		public TimeRange Opening { get; set; } = new TimeRange();

		[JsonPropertyName("ending")]
		public TimeRange Ending { get; set; } = new TimeRange();

		[JsonPropertyName("preview")]
		public ReleaseDataModelPoster Preview { get; set; } = new ReleaseDataModelPoster();

	}

}
