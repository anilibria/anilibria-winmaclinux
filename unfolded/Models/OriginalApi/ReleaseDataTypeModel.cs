using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

	public record ReleaseDataTypeModel
	{

		[JsonPropertyName("value")]
		public string Value { get; init; } = "";

	}

}