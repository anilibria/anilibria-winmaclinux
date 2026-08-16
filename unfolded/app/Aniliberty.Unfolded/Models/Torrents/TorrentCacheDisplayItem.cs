namespace Aniliberty.Unfolded.Models.Torrents
{

	public record TorrentCacheDisplayItem
	{

		public string Codec { get; init; } = "";

		public required int ReleaseId { get; init; }

		public int CountVideos { get; init; }

		public int CountDownloaded { get; init; }

	}

}
