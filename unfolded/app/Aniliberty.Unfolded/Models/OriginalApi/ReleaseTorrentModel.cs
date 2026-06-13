using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

	internal record ReleaseTorrentModel
	{

		[JsonPropertyName("hash")]
		public string Hash { get; init; } = "";

		[JsonPropertyName("id")]
		public int Id { get; init; }

		[JsonPropertyName("marnet")]
		public string Magnet { get; init; } = "";

		[JsonPropertyName("filename")]
		public string Filename { get; init; } = "";

		[JsonPropertyName("description")]
		public string Description { get; init; } = "";

		[JsonPropertyName("quality")]
		public StringValueItem Quality { get; init; } = new StringValueItem();

		[JsonPropertyName("codec")]
		public StringValueItem Codec { get; init; } = new StringValueItem();

		[JsonPropertyName("type")]
		public StringValueItem Type { get; init; } = new StringValueItem();

		[JsonPropertyName("size")]
		public long Size { get; init; }

		[JsonPropertyName("seeders")]
		public int Seeders { get; init; }

		[JsonPropertyName("updated_at")]
		public string UpdatedAt { get; init; } = "";

	}

}
