using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

	public class LoginPassAuthResponseModel
	{

		[JsonPropertyName("token")]
		public string Token { get; set; } = "";

	}

}
