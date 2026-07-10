using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

	public record FavoritesEditModel
	{
		[JsonPropertyName("release_id")]
		public int ReleaseId { get; set; }

	}

}
