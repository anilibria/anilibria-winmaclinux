namespace Aniliberty.Unfolded.Models.Settings
{

	public class ApplicationSettingsTorrentModel
	{

		public string PathToDownloads { get; set; } = "";

		public bool UseInternalTorrectClient { get; set; }

		public IEnumerable<string> CodecPrefference { get; set; } = ["avc", "hevc", "av1"];

		public int StartedSection { get; set; } = 0;

		public int StartedSubSection { get; set; } = -1;

		public int OpenLinkMode { get; set; } = 0;

	}

}