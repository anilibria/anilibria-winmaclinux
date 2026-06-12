using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

    internal record ReleaseEpisodeModel
    {

        public string Id { get; set; } = "";

        [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
        public string Name { get; set; } = "";

        [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
        public string NameEnglish { get; set; } = "";

        public decimal Ordinal { get; set; }

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

        public long Duration { get; set; }

        [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
        public string RutubeId { get; set; } = "";

        [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
        public string YoutubeId { get; set; } = "";

        public string UpdatedAt { get; set; } = "";

        public TimeRange Opening { get; set; } = new TimeRange();

        public TimeRange Ending { get; set; } = new TimeRange();

        public ReleaseDataModelPoster Preview { get; set; } = new ReleaseDataModelPoster();

    }

}
