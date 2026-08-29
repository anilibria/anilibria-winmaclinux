using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

	internal record StringValueItem
	{

		[JsonPropertyName("value")]
		public string Value { get; set; } = "";

		[JsonPropertyName("description")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public string Description { get; set; } = "";

		[JsonPropertyName("label")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public string Label { get; set; } = "";

	}

}
