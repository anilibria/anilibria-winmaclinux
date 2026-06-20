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

		static HashSet<int> m_favorites = new HashSet<int>();

		static HashSet<int> m_hidedReleases = new HashSet<int>();

		static List<int> m_openHistory = new List<int>();

		static List<int> m_seenHistory = new List<int>();

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapGet("/releases/release", ([FromQuery] int id) => Release(id));
			app.MapPost("/releases/list", ([FromBody] ReleasesListFiltersModel model) => List(model));
		}

		internal static async Task Initialize()
		{
			var path = GlobalConfig.PathToCache();
			if (Synchronize.IsEmptyTypes(GlobalConfig.PathToCache())) return; // mean no cache need to first synchronized

			if (Synchronize.MetadataExists(path))
			{
				var metadata = await Synchronize.ReadMetadata(path);
				await ReadReleases(metadata, path);
			}
		}

		internal static IResult Release(int id)
		{
			if (m_releasesMap.ContainsKey(id)) return Results.Json(m_releasesMap[id], AppJsonSerializerContext.Default);

			return Results.NotFound();
		}

		internal static IResult List(ReleasesListFiltersModel model)
		{
			var filteredItems = FilterReleases(model);

			return Results.Json(filteredItems, AppJsonSerializerContext.Default);
		}

		private static IEnumerable<ReleaseSaveModel> FilterReleases(ReleasesListFiltersModel model)
		{
			return m_releases
				.Where(
					a =>
					{
						if (!FilterBySection(model.Section, a)) return false;

						return true;
					}
				)
				.ToList();
		}

		private static bool FilterBySection(ReleasesListFiltersSection section, ReleaseSaveModel release)
		{
			if (m_hidedReleases.Contains(release.Id)) return false;

			switch (section)
			{
				case ReleasesListFiltersSection.All: return true;
				case ReleasesListFiltersSection.Favorites: return m_favorites.Contains(release.Id);
				case ReleasesListFiltersSection.Schedule: return release.PublishDay is not null;
				case ReleasesListFiltersSection.OpenHistory: return m_openHistory.Contains(release.Id);
				case ReleasesListFiltersSection.SeenHistory: return m_seenHistory.Contains(release.Id);
				default: throw new NotSupportedException("Section not supported!");
			}
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
