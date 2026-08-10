namespace Aniliberty.Unfolded.Models.Torrents
{

	public record TorrentCacheItem
	{

		public string Description { get; set; } = "";

		public required int ReleaseId { get; set; }

		public int CountVideos { get; set; }

		public string Path { get; set; } = "";

		public string MetadataPath { get; set; } = "";

	}

}