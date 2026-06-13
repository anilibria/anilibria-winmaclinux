using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

	public record ReleaseDataNameModel
	{

		[JsonPropertyName("main")]
		public string Main { get; init; } = "";

		[JsonPropertyName("english")]
		public string English { get; init; } = "";

		[JsonPropertyName("alternative")]
		public string Alternative { get; init; } = "";

	}

}