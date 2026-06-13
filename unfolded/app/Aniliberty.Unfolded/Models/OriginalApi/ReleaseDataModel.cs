using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

    public record ReleaseDataModel
    {

        [JsonPropertyName("id")]
        public int Id { get; init; }

		[JsonPropertyName("year")]
		public int Year { get; init; }

		[JsonPropertyName("alias")]
		public string Alias { get; init; } = "";

		[JsonPropertyName("fresh_at")]
		public string FreshAt { get; init; } = "";

		[JsonPropertyName("is_ongoing")]
		public bool IsOngoing { get; init; }

		[JsonPropertyName("description")]
		public string Description { get; init; } = "";

		[JsonPropertyName("episodes_total")]
		public int? EpisodesTotal { get; init; } = null;

		[JsonPropertyName("episodes_are_unknown")]
		public bool EpisodesAreUnknown { get; init; }

		[JsonPropertyName("is_blocked_by_copyrights")]
		public bool IsBlockedByCopyrights { get; init; }

		[JsonPropertyName("is_blocked_by_geo")]
		public bool IsBlockedByGeo { get; init; }

		[JsonPropertyName("average_duration_of_episode")]
		public int? AverageDurationOfEpisode { get; init; } = null;

		[JsonPropertyName("added_in_users_favorites")]
		public int? AddedInUsersFavorites { get; init; } = null;

		[JsonPropertyName("is_in_production")]
		public bool IsInProduction { get; init; }

		[JsonPropertyName("notification")]
		public string? Notification { get; set; }

		[JsonPropertyName("type")]
		public ReleaseDataTypeModel Type { get; init; } = new ReleaseDataTypeModel();

		[JsonPropertyName("name")]
		public ReleaseDataNameModel Name { get; init; } = new ReleaseDataNameModel();

		[JsonPropertyName("season")]
		public ReleaseDataSeasonModel Season { get; init; } = new ReleaseDataSeasonModel();

		[JsonPropertyName("poster")]
		public ReleaseDataModelPoster Poster { get; init; } = new ReleaseDataModelPoster();

		[JsonPropertyName("age_rating")]
		public ReleaseDataModelAgeRating AgeRating { get; init; } = new ReleaseDataModelAgeRating();

		[JsonPropertyName("publich_day")]
		public ReleaseDataModelPublishDay PublishDay { get; init; } = new ReleaseDataModelPublishDay();

		[JsonPropertyName("genres")]
		public IEnumerable<ReleaseDataModelGenre> Genres { get; init; } = [];

    }

}