namespace Aniliberty.Unfolded.Models.Releases
{

	public class ReleaseDisplayTorrentModel
	{

		public long Time { get; set; }

		public string Hash { get; init; } = "";

		public string Magnet { get; init; } = "";

		public string Filename { get; init; } = "";

		public string Description { get; init; } = "";

		public string Quality { get; init; } = "";

		public string Codec { get; init; } = "";

		public string Type { get; init; } = "";

		public long Size { get; init; }

		public string DisplayForm { get; set; } = "";

	}

}
