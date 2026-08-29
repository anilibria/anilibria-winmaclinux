using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

	public record ReleaseDataModelGenre
	{

		[JsonPropertyName("id")]
		public int Id { get; init; }

		[JsonPropertyName("total_releases")]
		public int TotalReleases { get; init; }

	}

}