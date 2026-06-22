using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{
	public class LoginPassAuthModel
	{

		[JsonPropertyName("login")]
		public string Login { get; set; } = "";

		[JsonPropertyName("password")]
		public string Password { get; set; } = "";

	}
}
