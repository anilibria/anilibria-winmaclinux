using Aniliberty.Unfolded.Configuration;
using Aniliberty.Unfolded.Helpers;
using Aniliberty.Unfolded.Models.CacheModels;
using Aniliberty.Unfolded.Models.Releases;
using Microsoft.AspNetCore.Mvc;
using System.Text;
using static Aniliberty.Unfolded.Helpers.JsonHelpers;

namespace Aniliberty.Unfolded.Routes
{
	public static class Releases
	{

		static List<ReleaseSaveEpisodeModel> m_episodes = [];

		static List<ReleaseSaveModel> m_releases = [];

		static Dictionary<int, ReleaseSaveModel> m_releasesMap = [];

		static List<ReleaseTorrentSaveModel> m_torrents = [];

		static HashSet<string> m_seenEpisodes = [];

		static HashSet<int> m_favorites = new HashSet<int>();

		static HashSet<int> m_localFavorites = new HashSet<int>();

		static HashSet<int> m_hidedReleases = new HashSet<int>();

		static List<int> m_openHistory = new List<int>();

		static List<int> m_seenHistory = new List<int>();

		static List<int> m_notificationReleases = new List<int>();

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapGet("/releases/release", ([FromQuery] int id) => Release(id));
			app.MapPost("/releases/list", ([FromBody] ReleasesListFiltersModel model) => List(model));
			app.MapGet("/releases/marks", () => Marks());
		}

		internal static async Task Initialize()
		{
			Console.WriteLine("Initialize Releases...");

			var path = GlobalConfig.PathToCache();
			if (Synchronize.IsEmptyTypes(GlobalConfig.PathToCache()))
			{
				Console.WriteLine("Types file not found, no need to do anything else.");
				return; // mean no cache need to first synchronized
			}

			await ReadReleases();

			var userDataPath = Path.Combine(path, "userdata.cache");
			if (File.Exists(userDataPath))
			{
				Console.WriteLine("User Data...");
				var content = await File.ReadAllTextAsync(userDataPath);
				var userCollections = DeserializeFromJson<UserCollections>(content);
			}

			Console.WriteLine("Initialize Releases completed!");
		}

		internal static async Task ReadReleases()
		{
			var path = GlobalConfig.PathToCache();
			if (Synchronize.MetadataExists(path))
			{
				Console.WriteLine("Reading metadata...");
				var metadata = await Synchronize.ReadMetadata(path);
				await ReadReleases(metadata, path);
			}
		}

		internal static async Task CheckNotifications(IEnumerable<ReleaseForCompare> newReleases)
		{
			if (!m_releases.Any()) return;

			var torrentMap = m_torrents.ToLookup(a => a.ReleaseId, a => a.Size);
			var currentReleases = m_releases
				.Select(a => new ReleaseForCompare { Id = a.Id, CountVideos = a.CountVideos, TorrentsSize = torrentMap[a.Id].Any() ? torrentMap[a.Id].Sum() : 0 })
				.ToDictionary(a => a.Id);

			var countNewReleases = 0;
			var countReleasesSeries = 0;
			var countReleasesTorrents = 0;
			foreach (var newRelease in newReleases)
			{
				if (!currentReleases.ContainsKey(newRelease.Id))
				{
					m_notificationReleases.Add(newRelease.Id);
					countNewReleases++;
					continue;
				}

				var currentRelease = currentReleases[newRelease.Id];
				if (currentRelease.CountVideos < newRelease.CountVideos)
				{
					m_notificationReleases.Add(newRelease.Id);
					countReleasesSeries++;
				}
				if (currentRelease.TorrentsSize != newRelease.TorrentsSize)
				{
					m_notificationReleases.Add(newRelease.Id);
					countReleasesTorrents++;
				}
			}

			var messages = new StringBuilder();
			if (countNewReleases > 0) messages.Append($"Новых релизов {countNewReleases} ");
			if (countReleasesSeries > 0) messages.Append($"Новые серии в релизах {countReleasesSeries} ");
			if (countReleasesTorrents > 0) messages.Append($"Обновленные торренты в релизах {countReleasesTorrents}");

			await WebSocketHub.SendMessage("ntc", messages.ToString());
		}

		internal static async Task SaveUserData(IEnumerable<int> favorites, IEnumerable<IEnumerable<object>> seenMarks)
		{
			foreach (var favorite in favorites) m_favorites.Add(favorite);

			foreach (var seenMark in seenMarks)
			{
				if (seenMark.Count() < 3) continue;

				var identifier = seenMark.ElementAt(0).ToString();
				//var time = Convert.ToInt64(seenMark.ElementAt(1)); not sure it need
				var status = seenMark.ElementAt(2)?.ToString()?.ToLowerInvariant() == "true";

				if (status == true) m_seenEpisodes.Add(identifier ?? "");
			}

			var saveModel = new UserCollections
			{
				CloudFavorites = favorites,
				LocalFavorites = m_localFavorites,
				SeenEpisodes = m_seenEpisodes
			};
			await File.WriteAllTextAsync(Path.Combine(GlobalConfig.PathToCache(), "userdata.cache"), SerializeToJson(saveModel));
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

		internal static IResult Marks()
		{
			Dictionary<int, int> releaseSeries = new Dictionary<int, int>();
			var fullReleaseSeens = new HashSet<int>();
			foreach (var releaseEpisodes in m_episodes)
			{
				if (!m_releasesMap.ContainsKey(releaseEpisodes.ReleaseId)) continue;

				var release = m_releasesMap[releaseEpisodes.ReleaseId];

				var countSeens = releaseEpisodes.Items.Count(a => m_seenEpisodes.Contains(a.Id));
				if (countSeens >= release.CountVideos)
				{
					fullReleaseSeens.Add(releaseEpisodes.ReleaseId);
				}
				else
				{
					releaseSeries.Add(releaseEpisodes.ReleaseId, countSeens);
				}
			}
			var result = new MarksModel
			{
				Favorites = m_favorites,
				SeenSeries = releaseSeries,
				FullSeenReleases = fullReleaseSeens
			};

			return Results.Json(result, AppJsonSerializerContext.Default);
		}

		private static IEnumerable<ReleaseSaveModel> FilterReleases(ReleasesListFiltersModel model)
		{
			var seenEpisodes = new Dictionary<int, int>();
			foreach (var releaseEpisodes in m_episodes)
			{
				if (!m_releasesMap.ContainsKey(releaseEpisodes.ReleaseId)) continue;

				var release = m_releasesMap[releaseEpisodes.ReleaseId];

				var countSeens = releaseEpisodes.Items.Count(a => m_seenEpisodes.Contains(a.Id));
				seenEpisodes.Add(releaseEpisodes.ReleaseId, countSeens);
			}

			return SortingReleases(
				m_releases
				.Where(
					a =>
					{
						if (!string.IsNullOrEmpty(model.Filter) &&
							!(a.Title.ToLowerInvariant().Contains(model.Filter) || a.OriginalName.ToLowerInvariant().Contains(model.Filter)))
						{
							return false;
						}

						if (!FilterBySection(model.Section, a, seenEpisodes)) return false;

						return true;
					}
				),
				model
			);
		}

		private static IEnumerable<ReleaseSaveModel> SortingReleases(IEnumerable<ReleaseSaveModel> releases, ReleasesListFiltersModel model)
		{
			switch (model.SortingField)
			{
				case ReleasesListFiltersModelSortingField.DateUpdate: return model.SortingDescending == true ? releases.OrderByDescending(a => a.Timestamp) : releases.OrderBy(a => a.Timestamp);
				case ReleasesListFiltersModelSortingField.Name: return model.SortingDescending == true ? releases.OrderByDescending(a => a.Title) : releases.OrderBy(a => a.Title);
				case ReleasesListFiltersModelSortingField.OriginalName: return model.SortingDescending == true ? releases.OrderByDescending(a => a.OriginalName) : releases.OrderBy(a => a.OriginalName);
				case ReleasesListFiltersModelSortingField.Rating: return model.SortingDescending == true ? releases.OrderByDescending(a => a.Rating) : releases.OrderBy(a => a.Rating);
				case ReleasesListFiltersModelSortingField.Status: return model.SortingDescending == true ? releases.OrderByDescending(a => a.Status) : releases.OrderBy(a => a.Status);
				case ReleasesListFiltersModelSortingField.Season: return model.SortingDescending == true ? releases.OrderByDescending(a => a.Season) : releases.OrderBy(a => a.Season);
				case ReleasesListFiltersModelSortingField.ReleaseSeries: return releases;
				case ReleasesListFiltersModelSortingField.SeenHistory: return releases;
				case ReleasesListFiltersModelSortingField.OpenHistory: return releases;
				case ReleasesListFiltersModelSortingField.ScheduleDay: return releases;
				case ReleasesListFiltersModelSortingField.Favorite: return releases;
				case ReleasesListFiltersModelSortingField.SeenMark: return releases;
				case ReleasesListFiltersModelSortingField.Year: return model.SortingDescending == true ? releases.OrderByDescending(a => a.Year) : releases.OrderBy(a => a.Year);
				case ReleasesListFiltersModelSortingField.History: return releases;
				default: throw new Exception("Not supported sorting field!");
			}
		}

		private static bool FilterBySection(ReleasesListFiltersSection section, ReleaseSaveModel release, Dictionary<int, int> seens)
		{
			if (m_hidedReleases.Contains(release.Id)) return false;

			switch (section)
			{
				case ReleasesListFiltersSection.All: return true;
				case ReleasesListFiltersSection.Favorites: return m_favorites.Contains(release.Id);
				case ReleasesListFiltersSection.Schedule: return release.PublishDay is not null;
				case ReleasesListFiltersSection.History: return m_openHistory.Contains(release.Id) || m_seenHistory.Contains(release.Id);
				case ReleasesListFiltersSection.OpenHistory: return m_openHistory.Contains(release.Id);
				case ReleasesListFiltersSection.SeenHistory: return m_seenHistory.Contains(release.Id);
				case ReleasesListFiltersSection.Notifications: return m_notificationReleases.Contains(release.Id);
				case ReleasesListFiltersSection.Seens: return seens.Keys.Contains(release.Id);
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
