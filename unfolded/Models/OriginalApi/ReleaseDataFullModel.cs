using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

	internal record ReleaseDataFullModel : ReleaseDataModel
	{

		[JsonPropertyName("episodes")]
		public IEnumerable<ReleaseEpisodeModel> Episodes { get; set; } = Enumerable.Empty<ReleaseEpisodeModel>();

		[JsonPropertyName("members")]
		public IEnumerable<ReleaseMemberModel> Members { get; set; } = Enumerable.Empty<ReleaseMemberModel>();

		[JsonPropertyName("torrents")]
		public IEnumerable<ReleaseTorrentModel> Torrents { get; set; } = Enumerable.Empty<ReleaseTorrentModel>();

	}

}
