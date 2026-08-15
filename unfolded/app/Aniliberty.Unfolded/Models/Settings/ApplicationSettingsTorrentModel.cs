namespace Aniliberty.Unfolded.Models.Settings
{

	public class ApplicationSettingsTorrentModel
	{

		public string PathToDownloads { get; set; } = "";

		public bool UseInternalTorrectClient { get; set; }

		public IEnumerable<string> CodecPrefference { get; set; } = ["avc", "hevc", "av1"];

	}

}