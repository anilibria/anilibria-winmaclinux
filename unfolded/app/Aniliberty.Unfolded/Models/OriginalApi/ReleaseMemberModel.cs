using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

    internal record ReleaseMemberModel
    {

		[JsonPropertyName("nickname")]
		public string Nickname { get; set; } = "";

		[JsonPropertyName("role")]
		public StringValueItem Role { get; set; } = new StringValueItem();

    }

}
