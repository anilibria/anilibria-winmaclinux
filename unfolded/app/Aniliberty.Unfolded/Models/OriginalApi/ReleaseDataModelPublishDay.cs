using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

	public record ReleaseDataModelPublishDay
	{

		[JsonPropertyName("value")]
		public int Value { get; init; }

	}

}