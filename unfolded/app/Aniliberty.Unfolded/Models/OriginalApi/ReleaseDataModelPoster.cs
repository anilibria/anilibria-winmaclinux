using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

	public record ReleaseDataModelPoster
	{

		[JsonPropertyName("src")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
		public string Src { get; init; } = "";

		[JsonIgnore]
		[JsonPropertyName("thumbnail")]
		public string Thumbnail { get; init; } = "";

	}

}