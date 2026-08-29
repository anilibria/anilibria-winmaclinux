using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

	public record SeenMarkEditModel
	{

		[JsonPropertyName("time")]
		public double Time { get; set; }

		[JsonPropertyName("is_watched")]
		public bool IsWatched { get; set; }

		[JsonPropertyName("release_episode_id")]
		public string EpisodeId { get; set; } = "";

	}

}
