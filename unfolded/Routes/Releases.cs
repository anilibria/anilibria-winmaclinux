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

		static List<int> m_notificationReleases = new List<int>();

		static ReleaseDictionaries m_releaseDictionaries = new ReleaseDictionaries();

		static string m_notificationMessage = "";

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapGet("/releases/release", ([FromQuery] int id) => Release(id));
			app.MapPost("/releases/list", ([FromBody] ReleasesListFiltersModel model) => List(model));
			app.MapPost("/releases/random", ([FromBody] ReleasesListFiltersModel? model) => RandomRelease(model));
			app.MapGet("/releases/marks", ([FromQuery] int[]? onlyForReleases) => Marks(onlyForReleases));
			app.MapPost("/releases/seenEpisodes", ([FromBody] int[] releases) => SeenEpisodes(releases));
			app.MapGet("/releases/episodes", (int releaseId) => Episodes(releaseId));
			app.MapGet("/releases/torrents", (int id) => Torrents(id));
			app.MapGet("/releases/openmagnet", (string magnet) => OpenMagnet(magnet));
			app.MapGet("/releases/franchise", (int id) => Franchise(id));
			app.MapGet("/releases/notifications", () => Results.Content(m_notificationMessage));
			app.MapGet("/releases/dictionaries", () => Results.Json(m_releaseDictionaries, AppJsonSerializerContext.Default));
			app.MapPost("/releases/addseens", (IHttpClientFactory clientFactory, HttpContext context, [FromBody] int[] ids) => AddReleasesToSeens(clientFactory, context, ids));
			app.MapPost("/releases/removeseens", (IHttpClientFactory clientFactory, HttpContext context, [FromBody] int[] ids) => RemoveReleasesToSeens(clientFactory, context, ids));
			app.MapPost("/releases/posters", ([FromBody] int[] ids) => ReleasePosters(ids));
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

		internal static bool IsEmptyData() => !m_releases.Any();

		internal static bool IsEmptyReleaseSeries() => !m_franchises.Any();

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

			m_releaseDictionaries.Genres = m_releases.SelectMany(a => a.Genres).Where(a => !string.IsNullOrEmpty(a)).ToHashSet();
			m_releaseDictionaries.Teams = m_releases.SelectMany(a => a.Team).Where(a => !string.IsNullOrEmpty(a)).OrderBy(a => a).ToHashSet();
			m_releaseDictionaries.Seasons = m_releases.Select(a => a.Season).Where(a => !string.IsNullOrEmpty(a)).ToHashSet();
			m_releaseDictionaries.Statuses = m_releases.Select(a => a.Status).Where(a => !string.IsNullOrEmpty(a)).ToHashSet();
			m_releaseDictionaries.Years = m_releases.Select(a => a.Year.ToString()).OrderByDescending(a => a).ToHashSet();
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

		internal static int GetNotificationMode() => Settings.Model.Releases.NotificationMode;

		internal static HashSet<int> GetFavorites() => m_favorites.Concat(m_localFavorites).ToHashSet();

		internal static async Task SetNotifications(IEnumerable<int> ids, int countNewReleases, int countNewEpisodes, int countNewTorrents)
		{
			if (Settings.Model.Releases.NotificationMode == 0) return;

			var messages = new StringBuilder();
			if (countNewReleases > 0) messages.Append($"Новых релизов {countNewReleases} ");
			if (countNewEpisodes > 0) messages.Append($"Новые серии в релизах {countNewEpisodes} ");
			if (countNewTorrents > 0) messages.Append($"Обновленные торренты в релизах {countNewTorrents}");

			m_notificationMessage = messages.ToString();
			m_notificationReleases = ids.ToList();

			await WebSocketHub.SendMessage("ntc", m_notificationMessage);

			if (ids.Any() && countNewTorrents > 0) await TorrentClient.RefreshTorrents(ids);
		}

		internal static async Task AddToFavorites(IEnumerable<int> ids, bool isLocal)
		{
			foreach (var id in ids)
			{
				if (isLocal)
				{
					m_localFavorites.Add(id);
				}
				else
				{
					m_favorites.Add(id);
				}
			}

			await ResaveUserData();
		}

		internal static async Task RemoveFromFavorites(IEnumerable<int> ids, bool isLocal)
		{
			foreach (var id in ids)
			{
				if (isLocal)
				{
					m_localFavorites.Remove(id);
				}
				else
				{
					m_favorites.Remove(id);
				}
			}

			await ResaveUserData();
		}

		private static async Task ResaveUserData()
		{
			var saveModel = new UserCollections
			{
				CloudFavorites = m_favorites,
				LocalFavorites = m_localFavorites,
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


		internal static IResult RandomRelease(ReleasesListFiltersModel? model)
		{
			var filteredItems = model is not null ? FilterReleases(model) : m_releases;
			var index = new Random((int)DateTime.UtcNow.TimeOfDay.TotalSeconds).Next(0, filteredItems.Count());
			return Results.Content(filteredItems.ElementAt(index).Id.ToString(), "application/json");
		}

		internal static IResult List(ReleasesListFiltersModel model)
		{
			var filteredItems = FilterReleases(model);

			return Results.Json(filteredItems, AppJsonSerializerContext.Default);
		}

		internal static IResult Marks(IEnumerable<int>? onlyForReleases)
		{
			var isOnlyReleases = onlyForReleases?.Any() ?? false;
			Dictionary<int, int> releaseSeries = new Dictionary<int, int>();
			var fullReleaseSeens = new HashSet<int>();
			foreach (var releaseEpisodes in m_episodes)
			{
				if (isOnlyReleases && onlyForReleases != null && !onlyForReleases.Contains(releaseEpisodes.ReleaseId)) continue;

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
				Favorites = isOnlyReleases && onlyForReleases != null ? m_favorites.Where(a => onlyForReleases.Contains(a)) : m_favorites,
				SeenSeries = releaseSeries,
				FullSeenReleases = fullReleaseSeens
			};

			return Results.Json(result, AppJsonSerializerContext.Default);
		}

		internal static IResult SeenEpisodes(IEnumerable<int> releases)
		{
			List<string> result = [];
			foreach (var release in releases)
			{
				var episodes = m_episodes.FirstOrDefault(a => a.ReleaseId == release);
				if (episodes != null) result.AddRange(episodes.Items.Where(a => m_seenEpisodes.Contains(a.Id)).Select(a => a.Id));
			}
			return Results.Json(result, AppJsonSerializerContext.Default);
		}

		internal static IEnumerable<ReleaseSaveModel> FilterReleases(ReleasesListFiltersModel model)
		{
			var seenEpisodes = new Dictionary<int, int>();
			foreach (var releaseEpisodes in m_episodes)
			{
				if (!m_releasesMap.ContainsKey(releaseEpisodes.ReleaseId)) continue;

				var release = m_releasesMap[releaseEpisodes.ReleaseId];

				var countSeens = releaseEpisodes.Items.Count(a => m_seenEpisodes.Contains(a.Id));
				seenEpisodes.Add(releaseEpisodes.ReleaseId, countSeens);
			}

			var mostPopular = m_releases
				.OrderByDescending(a => a.Rating)
				.Select(a => a.Id)
				.Take(50)
				.ToHashSet();
			var nowYear = DateTime.UtcNow.Year;
			var mostPopularInCurrentYear = m_releases
				.Where(a => a.Year == nowYear)
				.OrderByDescending(a => a.Rating)
				.Select(a => a.Id)
				.Take(30)
				.ToHashSet();
			var currentSeason = GetCurrentSeason(nowYear);

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
						if (CheckStringValue(model.Description, a.Description)) return false;
						if (CheckStringValue(model.Type, a.Type)) return false;
						if (CheckStringValue(model.Type, a.Type)) return false;
						if (CheckMultiStringValue(model.Team, a.Team, model.TeamOr ?? false)) return false;
						if (CheckMultiStringValue(model.Genres, a.Genres, model.GenresOr ?? false)) return false;
						if (CheckMultiStringSingleValue(model.Years, a.Year.ToString())) return false;
						if (CheckMultiStringSingleValue(model.Seasons, a.Season)) return false;
						if (CheckMultiStringSingleValue(model.Statuses, a.Status)) return false;
						if (CheckMultiIntValue(model.ScheduleDays, a.PublishDay)) return false;
						if (model.InFavorites.HasValue)
						{
							var inFavorite = m_favorites.Contains(a.Id) || m_localFavorites.Contains(a.Id);
							if (model.InFavorites.Value && !inFavorite) return false;
							if (!model.InFavorites.Value && inFavorite) return false;
						}
						if (model.PartOfReleases.HasValue)
						{
							var inFranchise = m_franchises.Any(b => b.Releases.Any(c => c.Id == a.Id));
							if (model.PartOfReleases.Value && !inFranchise) return false;
							if (!model.PartOfReleases.Value && inFranchise) return false;
						}

						if (!FilterBySection(model.Section, model.SubSection, a, seenEpisodes, mostPopular, mostPopularInCurrentYear, currentSeason, nowYear)) return false;

						return true;
					}
				),
				model
			);

			static bool CheckStringValue(string? filter, string? value)
			{
				if (string.IsNullOrEmpty(filter)) return false;
				if (string.IsNullOrEmpty(value)) return true;

				var valueLower = value.ToLowerInvariant();
				var filterLower = filter.ToLowerInvariant();

				var isMatched = valueLower.Contains(filterLower) || valueLower == filterLower;
				if (!isMatched) return true;

				return false;
			}

			static bool CheckMultiIntValue(IEnumerable<int>? filter, int? value)
			{
				if (filter is null || !filter.Any()) return false;
				if (!value.HasValue) return true;

				var isMatched = filter.Contains(value.Value);
				if (!isMatched) return true;

				return false;
			}

			static bool CheckMultiStringValue(IEnumerable<string>? filter, IEnumerable<string>? value, bool orAnd)
			{
				if (value is null) return false;
				if (filter is null || !filter.Any()) return false;
				filter = filter.Where(a => !string.IsNullOrEmpty(a)).ToList();
				if (!filter.Any()) return false;

				if (orAnd == false)
				{
					var founded = false;
					foreach (var filterItem in filter.Select(a => a.ToLowerInvariant()))
					{
						if (value.Where(a => a is not null).Any(a => a.ToLowerInvariant().Contains(filterItem)))
						{
							founded = true;
							break;
						}
					}
					if (!founded) return true;
				}
				else
				{
					var andFilter = filter
						.Select(a => a.ToLowerInvariant())
						.All(filterItemValue => value.Where(a => a is not null).Any(a => a.ToLowerInvariant().Contains(filterItemValue)));
					if (!andFilter) return true;
				}

				return false;
			}

			static bool CheckMultiStringSingleValue(IEnumerable<string>? filter, string? value)
			{
				if (string.IsNullOrEmpty(value)) return false;
				if (filter is null) return false;
				filter = filter.Where(a => !string.IsNullOrEmpty(a)).ToList();
				if (!filter.Any()) return false;

				var founded = false;
				foreach (var filterItem in filter.Where(a => !string.IsNullOrEmpty(a)).Select(a => a.ToLowerInvariant()))
				{
					if (value.ToLowerInvariant().Contains(filterItem))
					{
						founded = true;
						break;
					}
				}
				if (!founded) return true;

				return false;
			}

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

		private static bool FilterBySection(ReleasesListFiltersSection section, ReleasesListFiltersSubSection subSection, ReleaseSaveModel release, Dictionary<int, int> seens, HashSet<int> mostPopular, HashSet<int> mostPopularInCurrentYear, string? currentSeason, int nowYear)
		{
			if (AppData.InHidedReleases(release.Id) && section != ReleasesListFiltersSection.Seens && subSection != ReleasesListFiltersSubSection.Hided) return false;

			var releaseId = release.Id;

			switch (section)
			{
				case ReleasesListFiltersSection.All: return true;
				case ReleasesListFiltersSection.Favorites: return m_favorites.Contains(releaseId);
				case ReleasesListFiltersSection.Schedule: return release.PublishDay is not null;
				case ReleasesListFiltersSection.History:
					if (subSection == ReleasesListFiltersSubSection.OpenHistory) return AppData.IsInOnlyWatchHistory(releaseId);
					if (subSection == ReleasesListFiltersSubSection.SeenHistory) return AppData.IsInOnlyWatchVideoHistory(releaseId);
					return AppData.IsInWatchHistory(releaseId);
				case ReleasesListFiltersSection.Notifications: return m_notificationReleases.Contains(releaseId);
				case ReleasesListFiltersSection.Seens:
					if (subSection == ReleasesListFiltersSubSection.Seen) return seens.ContainsKey(releaseId) && seens[releaseId] == release.CountVideos;
					if (subSection == ReleasesListFiltersSubSection.SeenNow)
					{
						var countSeens = seens.ContainsKey(releaseId) ? seens[releaseId] : 0;
						return countSeens > 0 && countSeens < release.CountVideos;
					}
					if (subSection == ReleasesListFiltersSubSection.NotSeen) return !seens.ContainsKey(releaseId);
					if (subSection == ReleasesListFiltersSubSection.Hided) return AppData.InHidedReleases(releaseId);
					return true;
				case ReleasesListFiltersSection.Collections:
					if (subSection == ReleasesListFiltersSubSection.SeenToEnd) return seens.ContainsKey(releaseId) && seens[releaseId] == release.CountVideos && release.Status == "Озвучка завершена";
					if (subSection == ReleasesListFiltersSubSection.SeenNotToEnd) return seens.ContainsKey(releaseId) && seens[releaseId] < release.CountVideos && release.Status == "Озвучка завершена";
					if (subSection == ReleasesListFiltersSubSection.Films) return release.Type is not null && release.Type.ToLowerInvariant().Contains("фильм");
					if (subSection == ReleasesListFiltersSubSection.Completed) return release.Status == "Озвучка завершена";
					if (subSection == ReleasesListFiltersSubSection.PartOfRelease) return m_franchises.Any(a => a.Releases.Any(b => b.Id == release.Id));
					if (subSection == ReleasesListFiltersSubSection.MostPopular) return mostPopular.Contains(releaseId);
					if (subSection == ReleasesListFiltersSubSection.PopularInCurrentYear) return mostPopularInCurrentYear.Contains(releaseId);
					if (subSection == ReleasesListFiltersSubSection.InCinemaHall) return AppData.InCinemahall(releaseId);
					if (subSection == ReleasesListFiltersSubSection.CurrentSeason) return currentSeason is not null ? release.Season.ToLowerInvariant() == currentSeason.ToLowerInvariant() && release.Year == nowYear : false;
					if (subSection == ReleasesListFiltersSubSection.NotCurrentSeason) return currentSeason is not null ? release.Season.ToLowerInvariant() != currentSeason.ToLowerInvariant() && release.Year == nowYear : false;
					return true;
				default: throw new NotSupportedException("Section not supported!");
			}
		}

		private static string? GetCurrentSeason(int year)
		{
			return m_releases
				.Where(a => a.PublishDay is not null && a.Status == "Сейчас в озвучке" && a.Year == year)
				.GroupBy(a => a.Season)
				.Select(a => new { Season = a.Key, Count = a.Count() })
				.MaxBy(a => a.Count)?.Season;
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

			return Results.Json(new List<ReleaseDisplayEpisodeModel>().AsEnumerable(), AppJsonSerializerContext.Default);
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

			return Results.Json(new List<ReleaseDisplayTorrentModel>().AsEnumerable(), AppJsonSerializerContext.Default);
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

		private static async Task<IResult> AddReleasesToSeens(IHttpClientFactory clientFactory, HttpContext context, int[] ids)
		{
			var allEpisodes = new List<string>();

			foreach (var releaseEpisode in m_episodes)
			{
				if (!ids.Contains(releaseEpisode.ReleaseId)) continue;

				allEpisodes.AddRange(releaseEpisode.Items.Select(a => a.Id));
			}

			return await Synchronize.AddSeens(clientFactory, context, allEpisodes.ToArray());
		}

		private static async Task<IResult> RemoveReleasesToSeens(IHttpClientFactory clientFactory, HttpContext context, int[] ids)
		{
			var allEpisodes = new List<string>();

			foreach (var releaseEpisode in m_episodes)
			{
				if (!ids.Contains(releaseEpisode.ReleaseId)) continue;

				allEpisodes.AddRange(releaseEpisode.Items.Select(a => a.Id));
			}

			return await Synchronize.RemoveSeens(clientFactory, context, allEpisodes.ToArray());
		}

		private static IResult ReleasePosters(int[] ids)
		{
			var result = new List<ReleasePosterModel>();
			foreach (var id in ids)
			{
				if (m_releasesMap.TryGetValue(id, out var release))
				{
					result.Add(new ReleasePosterModel { Id = id, Poster = release.Poster });
				}
			}

			return Results.Json(result, AppJsonSerializerContext.Default);
		}

		internal static ReleaseTorrentSaveModel? GetReleaseTorrentByCodec(int releaseId, IEnumerable<string> codecPreferences)
		{
			var item = m_torrents.FirstOrDefault(a => a.ReleaseId == releaseId);
			if (item == null) return null;

			foreach (var codecPreference in codecPreferences)
			{
				var torrent = item.Items.FirstOrDefault(a => a.Codec.ToLowerInvariant().Contains(codecPreference));
				if (torrent != null) return torrent;
			}

			return null;
		}

		internal static ReleaseTorrentSaveModel? GetReleaseTorrentByHash(int releaseId, string hash)
		{
			var item = m_torrents.FirstOrDefault(a => a.ReleaseId == releaseId);
			if (item == null) return null;

			return item.Items.FirstOrDefault(a => a.Hash == hash);
		}

		internal static IEnumerable<ReleaseTorrentSaveModel> GetReleaseTorrentByReleaseId(int releaseId)
		{
			var item = m_torrents.FirstOrDefault(a => a.ReleaseId == releaseId);
			if (item == null) return [];

			return [.. item.Items];
		}

		internal static ReleaseSaveEpisodeModel? GetReleaseEpisodes(int releaseId) => m_episodes.FirstOrDefault(a => a.ReleaseId == releaseId);

		internal static int GetEpisodeRelease(string episodeId)
		{
			var item = m_episodes.FirstOrDefault(a => a.Items.Any(b => b.Id == episodeId));
			if (item is not null) return item.ReleaseId;

			return -1;
		}

	}

}
