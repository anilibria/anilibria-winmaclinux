using Aniliberty.Unfolded.Configuration;
using Aniliberty.Unfolded.Helpers;
using Aniliberty.Unfolded.Models.Releases;
using Microsoft.AspNetCore.Mvc;

namespace Aniliberty.Unfolded.Routes
{
	public static class Releases
	{

		static List<ReleaseSaveEpisodeModel> m_episodes = [];

		static List<ReleaseSaveModel> m_releases = [];

		static Dictionary<int, ReleaseSaveModel> m_releasesMap = [];

		static List<ReleaseTorrentSaveModel> m_torrents = [];

		static TypesResultModel m_types = new();

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapGet("/releases/release", ([FromQuery] int id) => Release(id));
			app.MapPost("/releases/list", ([FromBody] ReleasesListFiltersModel model) => List(model));
		}

		internal static async Task Initialize()
		{
			var path = GlobalConfig.PathToCache();
			if (!File.Exists(Path.Combine(path, "types.json"))) return; // mean no cache need to first synchronized

			m_types = await Synchronize.ReadTypes(path);
			var metadata = await Synchronize.ReadMetadata(path);
			await ReadReleases(metadata, path);
		}

		internal static IResult Release(int id)
		{
			if (m_releasesMap.ContainsKey(id)) return Results.Json(m_releasesMap[id], AppJsonSerializerContext.Default);

			return Results.NotFound();
		}

		internal static IResult List(ReleasesListFiltersModel model)
		{
			return Results.NotFound();
		}

		static async Task ReadReleases(MetadataModel metadata, string folderToSaveCacheFiles)
		{
			m_episodes.Clear();
			m_releases.Clear();
			m_torrents.Clear();

			var extension = ".cache";

			var countReleases = metadata.CountReleases;
			for (var i = 0; i < countReleases; i++)
			{
				var releasesPart = Path.Combine(folderToSaveCacheFiles, $"releases{i}{extension}");
				if (File.Exists(releasesPart))
				{
					var releasesPartJson = await File.ReadAllTextAsync(releasesPart);
					var deserialized = JsonHelpers.DeserializeFromJson<List<ReleaseSaveModel>>(releasesPartJson);
					if (deserialized != null) m_releases.AddRange(deserialized);
				}
			}

			var countEpisodes = metadata.CountEpisodes;
			for (var i = 0; i < countEpisodes; i++)
			{
				var episodesPart = Path.Combine(folderToSaveCacheFiles, $"episodes{i}{extension}");
				if (File.Exists(episodesPart))
				{
					var partJson = await File.ReadAllTextAsync(episodesPart);
					var deserialized = JsonHelpers.DeserializeFromJson<List<ReleaseSaveEpisodeModel>>(partJson);
					if (deserialized != null) m_episodes.AddRange(deserialized);
				}
			}

			var torrents = Path.Combine(folderToSaveCacheFiles, $"torrents{extension}");
			var fullJson = await File.ReadAllTextAsync(torrents);
			var deserializedTorrens = JsonHelpers.DeserializeFromJson<List<ReleaseTorrentSaveModel>>(fullJson);
			if (deserializedTorrens != null) m_torrents.AddRange(deserializedTorrens);

			m_releasesMap = m_releases.ToDictionary(a => a.Id);
		}

	}

}
