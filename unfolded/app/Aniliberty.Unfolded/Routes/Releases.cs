using Aniliberty.Unfolded.Configuration;
using Aniliberty.Unfolded.Helpers;
using Aniliberty.Unfolded.Models.CacheModels;
using Aniliberty.Unfolded.Models.Releases;
using Microsoft.AspNetCore.Mvc;
using System.Globalization;
using System.Text;
using static Aniliberty.Unfolded.Helpers.JsonHelpers;

namespace Aniliberty.Unfolded.Routes
{
	public static class Releases
	{

		static List<ReleaseSaveEpisodeModel> m_episodes = [];

		static List<ReleaseSaveModel> m_releases = [];

		static Dictionary<int, ReleaseSaveModel> m_releasesMap = [];

		static List<ReleaseTorrentsSaveModel> m_torrents = [];

		static List<ReleaseSeriesSaveModel> m_franchises = [];

		static HashSet<string> m_seenEpisodes = [];

		static HashSet<int> m_favorites = new HashSet<int>();

		static HashSet<int> m_localFavorites = new HashSet<int>();

		static HashSet<int> m_hidedReleases = new HashSet<int>();

		static List<int> m_openHistory = new List<int>();

		static List<int> m_seenHistory = new List<int>();

		static List<int> m_notificationReleases = new List<int>();

		static string m_notificationMessage = "";

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapGet("/releases/release", ([FromQuery] int id) => Release(id));
			app.MapPost("/releases/list", ([FromBody] ReleasesListFiltersModel model) => List(model));
			app.MapGet("/releases/marks", ([FromQuery] int[]? onlyForReleases) => Marks(onlyForReleases));
			app.MapGet("/releases/episodes", (int releaseId) => Episodes(releaseId));
			app.MapGet("/releases/torrents", (int id) => Torrents(id));
			app.MapGet("/releases/openmagnet", (string magnet) => OpenMagnet(magnet));
			app.MapGet("/releases/franchise", (int id) => Franchise(id));
			app.MapGet("/releases/notifications", () => Results.Content(m_notificationMessage));
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
				if (userCollections is not null)
				{
					m_favorites = userCollections.CloudFavorites.ToHashSet();
					m_localFavorites = userCollections.LocalFavorites.ToHashSet();
					m_seenEpisodes = userCollections.SeenEpisodes.ToHashSet();
				}
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

			var releasesSeriesPath = Path.Combine(path, "releaseseries.cache");
			if (File.Exists(releasesSeriesPath))
			{
				var releaseSeriesJson = await File.ReadAllTextAsync(releasesSeriesPath);
				m_franchises = DeserializeFromJson<List<ReleaseSeriesSaveModel>>(releaseSeriesJson) ?? new List<ReleaseSeriesSaveModel>();
			}
		}

		internal static IEnumerable<ReleaseSeriesReleaseSaveModel> GetReleasesPosterAndNames(IEnumerable<int> ids)
		{
			var result = new List<ReleaseSeriesReleaseSaveModel>(ids.Count());

			foreach (var id in ids)
			{
				if (m_releasesMap.TryGetValue(id, out var release))
				{
					result.Add(new ReleaseSeriesReleaseSaveModel { Id = release.Id, Poster = release.Poster, Title = release.Title });
				}
			}

			return result;
		}

		internal static async Task SetNotifications(IEnumerable<int> ids, int countNewReleases, int countNewEpisodes, int countNewTorrents)
		{
			if (Settings.Model.Releases.NotificationMode == 0) return;

			var onlyFavorites = Settings.Model.Releases.NotificationMode == 2;

			var currentReleases = ids
				.Where(a => onlyFavorites ? m_favorites.Contains(a) || m_localFavorites.Contains(a) : true)
				.ToDictionary(a => a);

			var messages = new StringBuilder();
			if (countNewReleases > 0) messages.Append($"Новых релизов {countNewReleases} ");
			if (countNewEpisodes > 0) messages.Append($"Новые серии в релизах {countNewEpisodes} ");
			if (countNewTorrents > 0) messages.Append($"Обновленные торренты в релизах {countNewTorrents}");

			m_notificationMessage = messages.ToString();

			await WebSocketHub.SendMessage("ntc", m_notificationMessage);
		}

		internal static async Task SaveOnlyFavorites(IEnumerable<int>? favorites, IEnumerable<int>? localFavorites)
		{
			var saveModel = new UserCollections
			{
				CloudFavorites = favorites is not null ? favorites : m_favorites,
				LocalFavorites = localFavorites is not null ? localFavorites : m_localFavorites,
				SeenEpisodes = m_seenEpisodes
			};
			await File.WriteAllTextAsync(Path.Combine(GlobalConfig.PathToCache(), "userdata.cache"), SerializeToJson(saveModel));
		}

		internal static async Task SaveOnlySeens(IEnumerable<string> seens)
		{
			var saveModel = new UserCollections
			{
				CloudFavorites = m_favorites,
				LocalFavorites = m_localFavorites,
				SeenEpisodes = seens
			};
			await File.WriteAllTextAsync(Path.Combine(GlobalConfig.PathToCache(), "userdata.cache"), SerializeToJson(saveModel));
		}

		internal static async Task SaveUserData(IEnumerable<int> favorites, IEnumerable<IEnumerable<object>> seenMarks)
		{
			m_favorites = [.. favorites];

			m_seenEpisodes = [];
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

		/// <summary>
		/// A quick way to add seen marks to memory instead of querying for all seen marks.
		/// </summary>
		/// <param name="ids">Episodes identifiers.</param>
		internal static async Task AddSeenMarksToMemory(string[] ids)
		{
			foreach (var id in ids)
			{
				if (!m_seenEpisodes.Contains(id)) m_seenEpisodes.Add(id);
			}

			await SaveOnlySeens(m_seenEpisodes);
		}

		/// <summary>
		/// A quick way to remove seen marks to memory instead of querying for all seen marks.
		/// </summary>
		/// <param name="ids">Episodes identifiers.</param>
		internal static async Task RemoveSeenMarksToMemory(string[] ids)
		{
			foreach (var id in ids)
			{
				if (m_seenEpisodes.Contains(id)) m_seenEpisodes.Remove(id);
			}

			await SaveOnlySeens(m_seenEpisodes);
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

		internal static IResult Marks(IEnumerable<int>? onlyForReleases)
		{
			Dictionary<int, int> releaseSeries = new Dictionary<int, int>();
			var fullReleaseSeens = new HashSet<int>();
			foreach (var releaseEpisodes in m_episodes)
			{
				if (onlyForReleases != null && !onlyForReleases.Contains(releaseEpisodes.ReleaseId)) continue;

				if (!m_releasesMap.ContainsKey(releaseEpisodes.ReleaseId)) continue;

				var release = m_releasesMap[releaseEpisodes.ReleaseId];

				var countSeens = releaseEpisodes.Items.Count(a => m_seenEpisodes.Contains(a.Id));
				if (countSeens >= release.CountVideos)
				{
					fullReleaseSeens.Add(releaseEpisodes.ReleaseId);
				}
				else
				{
					if (countSeens > 0) releaseSeries.Add(releaseEpisodes.ReleaseId, countSeens);
				}
			}
			var result = new MarksModel
			{
				Favorites = onlyForReleases != null ? m_favorites.Where(a => onlyForReleases.Contains(a)) : m_favorites,
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

		static internal (List<ReleaseSaveEpisodeModel> episodes, List<ReleaseSaveModel> releases, Dictionary<int, ReleaseSaveModel> m_releasesMap, List<ReleaseTorrentsSaveModel> m_torrents) OriginalCollections()
		{
			return (m_episodes, m_releases, m_releasesMap, m_torrents);
		}

		static async Task ReadReleases(MetadataModel metadata, string folderToSaveCacheFiles)
		{
			var episodes = new List<ReleaseSaveEpisodeModel>();
			var releases = new List<ReleaseSaveModel>();
			var allTorrents = new List<ReleaseTorrentsSaveModel>();

			var extension = ".cache";

			var countReleases = metadata.CountReleases;
			for (var i = 0; i < countReleases; i++)
			{
				var releasesPart = Path.Combine(folderToSaveCacheFiles, $"releases{i}{extension}");
				if (File.Exists(releasesPart))
				{
					var releasesPartJson = await File.ReadAllTextAsync(releasesPart);
					var deserialized = JsonHelpers.DeserializeFromJson<List<ReleaseSaveModel>>(releasesPartJson);
					if (deserialized != null) releases.AddRange(deserialized);
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
					if (deserialized != null) episodes.AddRange(deserialized);
				}
			}

			var torrents = Path.Combine(folderToSaveCacheFiles, $"torrents{extension}");
			var fullJson = await File.ReadAllTextAsync(torrents);
			var deserializedTorrens = JsonHelpers.DeserializeFromJson<List<ReleaseTorrentsSaveModel>>(fullJson);
			if (deserializedTorrens != null) allTorrents.AddRange(deserializedTorrens);

			m_episodes = episodes;
			m_releases = releases;
			m_torrents = allTorrents;

			m_releasesMap = m_releases.ToDictionary(a => a.Id);
		}

		internal static IResult Episodes(int releaseId)
		{
			var episodes = m_episodes
				.FirstOrDefault(a => a.ReleaseId == releaseId);
			if (episodes != null)
			{
				return Results.Json(
					episodes.Items.Select(a => new ReleaseDisplayEpisodeModel
					{
						Id = a.Id,
						Name = a.Name,
						Hls1080 = a.Hls1080,
						Hls480 = a.Hls480,
						Hls720 = a.Hls720,
						Ordinal = a.Ordinal,
						Preview = a.Poster,
						RutubeId = a.RutubeId,
						YoutubeId = a.YoutubeId,
						SortOrder = a.SortOrder,
						OpeningEnd = a.OpeningEnd,
						OpeningStart = a.OpeningStart,
						EndingEnd = a.EndingEnd,
						EndingStart = a.EndingStart
					}),
					AppJsonSerializerContext.Default
				);
			}

			return Results.Json(new List<ReleaseDisplayEpisodeModel>(), AppJsonSerializerContext.Default);
		}

		private static string GetSizeFromBytes(long bytes, int decimalPlaces = 1)
		{
			if (bytes < 0) throw new ArgumentOutOfRangeException(nameof(bytes));
			if (bytes == 0) return "0 B";

			string[] units = { "B", "KB", "MB", "GB", "TB", "PB", "EB" };

			double value = bytes;
			int unitIndex = 0;

			while (value >= 1024 && unitIndex < units.Length - 1)
			{
				value /= 1024;
				unitIndex++;
			}

			return value.ToString($"F{decimalPlaces}", CultureInfo.InvariantCulture) + " " + units[unitIndex];
		}

		internal static IResult Torrents(int releaseId)
		{
			var torrent = m_torrents.FirstOrDefault(a => a.ReleaseId == releaseId);

			if (torrent?.Items?.Any() == true)
			{
				return Results.Json(
					torrent.Items
						.Select(
							a => new ReleaseDisplayTorrentModel
							{
								Codec = a.Codec ?? "",
								Description = a.Description ?? "",
								Filename = a.Filename ?? "",
								Hash = a.Hash,
								Magnet = a.Magnet,
								Quality = a.Quality ?? "",
								Size = a.Size,
								Time = a.Time,
								Type = a.Type ?? "",
								DisplayForm = $"{(a.Quality ?? "")} {(a.Codec ?? "")} [{a.Description ?? ""}] {GetSizeFromBytes(a.Size)}"
							}
						),
					AppJsonSerializerContext.Default
				);
			}

			return Results.Json(new List<ReleaseDisplayTorrentModel>(), AppJsonSerializerContext.Default);
		}

		internal static IResult OpenMagnet(string magnet)
		{
			if (!magnet.StartsWith("magnet:?")) return Results.Ok();

			GlobalConfig.OpenPathInSystem(magnet);

			return Results.Ok();
		}

		internal static IResult Franchise(int id)
		{
			var franchise = m_franchises.FirstOrDefault(a => a.Releases.Any(b => b.Id == id));
			if (franchise is null) return Results.Content("null", contentType: "application/json");

			var countSeconds = TimeSpan.FromSeconds(franchise.CountSeconds);
			var model = new DisplayFranchiseModel
			{
				CountEpisodes = franchise.CountEpisodes,
				Title = franchise.Title,
				Poster = franchise.Poster,
				AllTime = $"{countSeconds.Hours} часов {countSeconds.Minutes} минут",
				Releases = franchise.Releases
					.Select(
						a => new DisplayFranchiseReleaseModel
						{
							Title = a.Title,
							Poster = a.Poster,
							ReleaseId = a.Id
						}
					)
			};

			return Results.Json(model, AppJsonSerializerContext.Default);
		}

	}

}
