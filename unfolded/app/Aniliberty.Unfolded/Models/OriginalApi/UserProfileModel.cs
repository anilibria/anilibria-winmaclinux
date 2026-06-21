using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{
	public class UserProfileModel
	{

		[JsonPropertyName("id")]
		public int Id { get; init; }

		[JsonPropertyName("login")]
		public string login { get; init; } = "";

		[JsonPropertyName("email")]
		public string email { get; init; } = "";

		[JsonPropertyName("nickname")]
		public string nickname { get; init; } = "";

		[JsonPropertyName("avatar")]
		public UserProfileAvatarModel Avatar { get; init; } = new UserProfileAvatarModel();

		[JsonPropertyName("torrents")]
		public UserProfileTorrentModel Torrents { get; init; } = new UserProfileTorrentModel();

		[JsonPropertyName("is_banned")]
		public bool is_banned { get; init; }

		[JsonPropertyName("created_at")]
		public DateTime created_at { get; init; }

		[JsonPropertyName("is_with_ads")]
		public bool is_with_ads { get; init; }

	}

	public record UserProfileAvatarModel
	{

		[JsonPropertyName("preview")]
		public string Preview { get; init; } = "";

	}

	public class UserProfileTorrentModel
	{

		[JsonPropertyName("passkey")]
		public string Passkey { get; init; } = "";

		[JsonPropertyName("uploaded")]
		public long? Uploaded { get; init; }

		[JsonPropertyName("downloaded")]
		public long? Downloaded { get; init; }

	}

}
