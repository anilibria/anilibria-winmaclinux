using Aniliberty.Unfolded.Models.OriginalApi;
using Aniliberty.Unfolded.Routes;

namespace Aniliberty.Unfolded.Models.Releases
{

	internal record ReleaseTorrentsSaveModel
	{

		public int ReleaseId { get; set; }


		public IEnumerable<ReleaseTorrentSaveModel> Items { get; set; } = Enumerable.Empty<ReleaseTorrentSaveModel>();

	}

	internal class ReleaseTorrentSaveModel
	{

		public long Time { get; set; }

		public string Hash { get; set; } = "";

		public int Id { get; set; }

		public string Magnet { get; set; } = "";

		public string Filename { get; set; } = "";

		public string Description { get; set; } = "";

		public string Quality { get; set; } = "";

		public string Codec { get; set; } = "";

		public string Type { get; set; } = "";

		public long Size { get; set; }

		public int Seeders { get; set; }

		internal void MapFromApiModel(ReleaseTorrentModel torrent)
		{
			Time = Synchronize.ParseDateTimeOffset(torrent.UpdatedAt);
			Hash = torrent.Hash;
			Id = torrent.Id;
			Magnet = torrent.Magnet;
			Filename = torrent.Filename;
			Description = torrent.Description;
			Quality = torrent.Quality.Description;
			Codec = torrent.Codec.Description;
			Type = torrent.Type.Description;
			Size = torrent.Size;
			Seeders = torrent.Seeders;
		}

		internal static ReleaseTorrentSaveModel CreateFromApi(ReleaseTorrentModel torrent)
		{
			var model = new ReleaseTorrentSaveModel();
			model.MapFromApiModel(torrent);
			return model;
		}

	}

}
