using Aniliberty.Unfolded.Configuration;
using Aniliberty.Unfolded.Helpers;
using Aniliberty.Unfolded.Models.OriginalApi;
using Aniliberty.Unfolded.Models.Releases;
using Microsoft.AspNetCore.Mvc;
using static Aniliberty.Unfolded.Helpers.JsonHelpers;

namespace Aniliberty.Unfolded.Routes
{

	public static class Synchronize
	{

		private static bool m_firstlyStarted = true;

		private static bool m_synchronizationStarted = false;

		private const string SynchronizedCommand = "sync";

		private const string SynchronizedCommandCompleted = "completed";

		private const string SynchronizedCommandUpToDate = "uptodate";

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapGet("/sync/full", ([FromServices] IHttpClientFactory clientFactory, [FromQuery] bool checkLatest = true) => Full(clientFactory, checkLatest));
			app.MapGet("/sync/user", ([FromServices] IHttpClientFactory clientFactory, HttpContext context) => User(clientFactory, context));
			app.MapGet("/sync/firststart", () => Results.Content(m_firstlyStarted ? "true" : "false"));
			app.MapGet("/sync/status", () => Results.Content(m_synchronizationStarted ? "true" : "false"));
			app.MapPost("/sync/addfavorites", (IHttpClientFactory clientFactory, HttpContext context, [FromBody] int[] ids) => AddFavorites(clientFactory, context, ids));
			app.MapPost("/sync/removefavorites", (IHttpClientFactory clientFactory, HttpContext context, [FromBody] int[] ids) => RemoveFavorites(clientFactory, context, ids));
			app.MapPost("/sync/addseens", (IHttpClientFactory clientFactory, HttpContext context, [FromBody] string[] ids) => AddSeens(clientFactory, context, ids));
			app.MapPost("/sync/removeseens", (IHttpClientFactory clientFactory, HttpContext context, [FromBody] string[] ids) => RemoveSeens(clientFactory, context, ids));
		}

		public static async Task<IResult> Full(IHttpClientFactory clientFactory, bool checkLatest)
		{
			var newSyncValue = true;
			var snapshotValue = m_synchronizationStarted;
			var originalSyncValue = Interlocked.CompareExchange(ref m_synchronizationStarted, newSyncValue, false);
			if (originalSyncValue == true) return Results.Conflict();

			try
			{
				var httpClient = clientFactory.CreateClient();
				httpClient.Timeout = TimeSpan.FromSeconds(20);
				var cacheFolder = GlobalConfig.PathToCache();

				if (IsEmptyTypes(cacheFolder)) await SaveTypes(httpClient, cacheFolder);

				await SaveFullReleases(httpClient, cacheFolder, checkLatest);
			}
			finally
			{
				m_synchronizationStarted = false;
				await WebSocketHub.SendMessage("sync", "failed");
			}

			return Results.Ok();
		}

		public static async Task<IResult> User(IHttpClientFactory clientFactory, HttpContext context)
		{
			if (m_firstlyStarted) m_firstlyStarted = false;

			var token = context.Request.Cookies?.FirstOrDefault(a => a.Key == Authorize.CookieName).Value ?? null;
			if (token == null) return Results.Unauthorized();

			var httpClient = clientFactory.CreateClient();
			httpClient.Timeout = TimeSpan.FromSeconds(20);

			var userData = await OriginalApiMaker.GetUserData(httpClient, token);
			var favorites = await OriginalApiMaker.GetUserFavorites(httpClient, token);
			var seens = await OriginalApiMaker.GetUserSeens(httpClient, token);

			MainMenu.SetUser(userData.nickname, userData.Avatar?.Preview ?? "");
			await Releases.SaveUserData(favorites, seens);

			await WebSocketHub.SendMessage("user", "opened");

			return Results.Ok();
		}

		public static bool IsEmptyTypes(string folderToSaveCacheFiles) => !File.Exists(Path.Combine(folderToSaveCacheFiles, "types.cache"));

		internal static long ConvertApiDateToUnixTimeStamp(string value)
		{
			try
			{
				return DateTimeOffset.Parse(value).ToUnixTimeSeconds();
			}
			catch
			{
				return -1;
			}
		}

		static public async Task<bool> SaveFullReleases(HttpClient httpClient, string folderToSaveCacheFiles, bool checkLatest)
		{
			await WebSocketHub.SendMessage(SynchronizedCommand, "started");

			long currentLastTimeStamp = -1;
			var metadataPath = Path.Combine(folderToSaveCacheFiles, "metadata");
			if (File.Exists(metadataPath))
			{
				var metadata = DeserializeFromJson<MetadataModel>(await File.ReadAllTextAsync(metadataPath));
				if (metadata != null) currentLastTimeStamp = metadata.LastReleaseTimeStamp;
			}

			var totalPages = 300;

			var allReleases = new List<ReleaseDataFullModel>();

			var allUpToDate = false;

			for (var i = 1; i < totalPages; i++)
			{
				Console.WriteLine($"Try to get page {i}");
				var page = await OriginalApiMaker.GetPageIds(i, httpClient, 50);
				if (totalPages == 300)
				{
					totalPages = page.Meta.Pagination.TotalPages + 1;
					Console.WriteLine("Total pages: " + totalPages);
				}

				if (!page.Data.Any()) break;

				var ids = page.Data.Select(a => a.Id).ToList();

				allReleases.AddRange((await OriginalApiMaker.GetReleasesInnerCollections(httpClient, ids)).Data);

				if (checkLatest && i == 1 && ConvertApiDateToUnixTimeStamp(allReleases.Where(a => a.FreshAt != null).OrderByDescending(a => a.FreshAt).First().FreshAt) == currentLastTimeStamp)
				{
					allUpToDate = true;
					break;
				}

				if (i % 2 == 0) await Task.Delay(1000);

				await WebSocketHub.SendMessage(SynchronizedCommand, "percent" + ((float)i / (float)totalPages) * 100);
			}

			if (allUpToDate)
			{
				Console.WriteLine("No need to synchronize releases all is up to date!");
				await WebSocketHub.SendMessage(SynchronizedCommand, SynchronizedCommandUpToDate);
				return false;
			}

			if (!allReleases.Any()) return false;

			allReleases = allReleases
				.Where(a => a.Episodes.Any() || a.Torrents.Any()) // stay only there where have episodes or torrents
				.ToList();

			var lastTimestamp = DateTimeOffset.Parse(allReleases.Where(a => a.FreshAt != null).OrderByDescending(a => a.FreshAt).First().FreshAt).ToUnixTimeSeconds();

			var types = await ReadTypes(folderToSaveCacheFiles);

			var (episodes, releases, releasesMap, torrents) = Releases.OriginalCollections();

			int countNewReleases = 0;
			int countNewEpisodes = 0;
			int countNewTorrents = 0;
			HashSet<int> notificationReleases = new HashSet<int>();
			var notificationMode = Releases.GetNotificationMode();
			var notificationOnylFavorites = notificationMode == 2;
			var favoriteReleases = Releases.GetFavorites();
			var needNotifications = releases.Any() && notificationMode != 0;

			foreach (var fullRelease in allReleases)
			{
				if (releasesMap.ContainsKey(fullRelease.Id))
				{
					var currentRelease = releasesMap[fullRelease.Id];
					currentRelease.MapFromApiModel(fullRelease, types);
					var needReleaseNotifications = needNotifications && (!notificationOnylFavorites || favoriteReleases.Contains(fullRelease.Id));

					if (fullRelease.Episodes?.Any() == true)
					{
						RemapEpisodes(fullRelease.Episodes);

						var releaseEpisodes = episodes.FirstOrDefault(a => a.ReleaseId == fullRelease.Id);
						if (releaseEpisodes == null)
						{
							episodes.Add(new ReleaseSaveEpisodeModel { ReleaseId = fullRelease.Id, Items = fullRelease.Episodes.Select(ReleaseSaveEpisodeItemModel.CreateFromApi).ToList() });
							if (needReleaseNotifications)
							{
								countNewEpisodes += 1;
								notificationReleases.Add(fullRelease.Id);
							}
						}
						else
						{
							if (needReleaseNotifications && releaseEpisodes.Items.Count() != fullRelease.Episodes.Count())
							{
								countNewEpisodes += 1;
								notificationReleases.Add(fullRelease.Id);
							}

							ReleaseSaveEpisodeItemModel.MapOrCreateFromApi(fullRelease.Episodes, releaseEpisodes.Items);
						}
					}
					if (fullRelease.Torrents?.Any() == true)
					{
						var torrentItems = fullRelease.Torrents.Select(ReleaseTorrentSaveModel.CreateFromApi).ToArray();
						var releaseTorrents = torrents.FirstOrDefault(a => a.ReleaseId == fullRelease.Id);
						if (releaseTorrents == null)
						{
							torrents.Add(new ReleaseTorrentsSaveModel { ReleaseId = fullRelease.Id, Items = torrentItems });
							if (needReleaseNotifications)
							{
								countNewTorrents += 1;
								notificationReleases.Add(fullRelease.Id);
							}
						}
						else
						{
							if (needReleaseNotifications && !releaseTorrents.Items.Select(a => a.Size).SequenceEqual(torrentItems.Select(a => a.Size)))
							{
								countNewTorrents += 1;
								notificationReleases.Add(fullRelease.Id);
							}
							releaseTorrents.Items = torrentItems;
						}
					}
				}
				else
				{
					var newRelease = new ReleaseSaveModel();
					newRelease.MapFromApiModel(fullRelease, types);
					releasesMap.Add(newRelease.Id, newRelease);
					releases.Add(newRelease);

					if (fullRelease.Episodes?.Any() == true)
					{
						RemapEpisodes(fullRelease.Episodes);
						episodes.Add(new ReleaseSaveEpisodeModel { ReleaseId = fullRelease.Id, Items = fullRelease.Episodes.Select(ReleaseSaveEpisodeItemModel.CreateFromApi).ToList() });
					}

					if (fullRelease.Torrents?.Any() == true)
					{
						torrents.Add(new ReleaseTorrentsSaveModel { ReleaseId = fullRelease.Id, Items = fullRelease.Torrents.Select(ReleaseTorrentSaveModel.CreateFromApi).ToArray() });
					}

					if (needNotifications)
					{
						countNewReleases += 1;
						notificationReleases.Add(newRelease.Id);
					}
				}
			}

			await SaveLoadedItemsToFiles(folderToSaveCacheFiles, releases, torrents, episodes, lastTimestamp);

			await WebSocketHub.SendMessage(SynchronizedCommand, SynchronizedCommandCompleted);

			await Releases.SetNotifications(notificationReleases, countNewReleases, countNewEpisodes, countNewTorrents);

			//save franchises if new releases was added
			if (countNewReleases > 0) await SaveReleaseSeries(httpClient, folderToSaveCacheFiles);

			return true;
		}

		public static async Task SaveReleaseSeries(HttpClient httpClient, string folderToSaveCacheFiles)
		{
			Console.WriteLine("Start synchronized franchises...");
			var franchises = await OriginalApiMaker.GetAllFranchises(httpClient);

			var result = new List<ReleaseSeriesSaveModel>();
			if (!franchises.Any()) return;

			Console.WriteLine($"Received {franchises.Count()} franchises");

			foreach (var franchise in franchises)
			{
				var releasesItem = await OriginalApiMaker.GetFranchisesReleases(httpClient, franchise.Id);
				if (releasesItem.FranchiseReleases.Count() <= 1) continue; //franchises with single release not actual

				var releaseIds = releasesItem.FranchiseReleases
					.OrderBy(a => a.SortOrder)
					.Select(a => a.ReleaseId)
					.ToArray();
				var releases = Releases.GetReleasesPosterAndNames(releaseIds);

				var model = new ReleaseSeriesSaveModel
				{
					CountReleases = releaseIds.Count(),
					Poster = franchise.Image.Preview,
					Releases = releases,
					Title = franchise.Name,
					CountSeconds = franchise.TotalDurationInSeconds ?? 0,
					CountEpisodes = franchise.TotalEpisodes ?? 0,
					Rating = franchise.Rating ?? 0
				};
				result.Add(model);
			}

			var path = Path.Combine(folderToSaveCacheFiles, "releaseseries.cache");
			Console.WriteLine($"Saving to file {Path.GetFullPath(path)} items");

			await File.WriteAllTextAsync(path, SerializeToJson(result));

			Console.WriteLine($"Franchises saved!");
		}

		public static async Task SaveTypes(HttpClient httpClient, string folderToSaveCacheFiles)
		{
			Console.WriteLine("Start synchronized types...");

			var ageRatings = await OriginalApiMaker.GetAgeRatings(httpClient);
			var genres = await OriginalApiMaker.GetGenres(httpClient);
			var seasons = await OriginalApiMaker.GetSeasons(httpClient);
			var types = await OriginalApiMaker.GetTypes(httpClient);

			Console.WriteLine($"Received {ageRatings.Count()} ratings items");
			Console.WriteLine($"Received {genres.Count()} genres items");
			Console.WriteLine($"Received {seasons.Count()} seasons items");
			Console.WriteLine($"Received {types.Count()} types items");

			var result = new TypesResultModel
			{
				AgeRatings = ageRatings,
				Genres = genres,
				Seasons = seasons,
				Types = types
			};

			var jsonContent = SerializeToJson(result);

			var path = Path.Combine(folderToSaveCacheFiles, "types.cache");
			Console.WriteLine($"Saving to file {Path.GetFullPath(path)} items");

			await File.WriteAllTextAsync(path, jsonContent);

			Console.WriteLine($"Types saved!");
		}

		internal static bool MetadataExists(string folderToSaveCacheFiles)
		{
			var metadataPath = Path.Combine(folderToSaveCacheFiles, "metadata");
			return File.Exists(metadataPath);
		}

		internal static async Task<MetadataModel> ReadMetadata(string folderToSaveCacheFiles)
		{
			var metadataPath = Path.Combine(folderToSaveCacheFiles, "metadata");
			if (!File.Exists(metadataPath))
			{
				var errorMessage = $"Metadata file is not exists!";
				Console.WriteLine(errorMessage);
				throw new Exception(errorMessage);
			}

			var metadata = DeserializeFromJson<MetadataModel>(await File.ReadAllTextAsync(metadataPath));
			if (metadata == null)
			{
				var errorMessage = "Can't read metadata file, please check if it file is correct!";
				Console.WriteLine(errorMessage);
				throw new Exception(errorMessage);
			}

			return metadata;
		}

		internal static async Task<TypesResultModel> ReadTypes(string folderToSaveCacheFiles)
		{
			var pathToTypes = Path.Combine(folderToSaveCacheFiles, "types.cache");
			if (!File.Exists(pathToTypes))
			{
				var errorMessage = $"File types.json not found by path {Path.GetFullPath(pathToTypes)}.";
				Console.WriteLine(errorMessage);
				throw new Exception(errorMessage);
			}
			var types = DeserializeFromJson<TypesResultModel>(await File.ReadAllTextAsync(pathToTypes));
			if (types == null)
			{
				var errorMessage = $"Content of types.json is corrupt.";
				Console.WriteLine(errorMessage);
				throw new Exception(errorMessage);
			}

			return types;
		}

		internal static long ParseDateTimeOffset(string value)
		{
			if (string.IsNullOrEmpty(value)) return 0;

			try
			{
				return DateTimeOffset.Parse(value).ToUnixTimeSeconds();
			}
			catch
			{
				return 0;
			}
		}

		static void RemapEpisodes(IEnumerable<ReleaseEpisodeModel> episodes)
		{
			foreach (var collection in episodes)
			{
				if (collection.Preview?.Thumbnail?.Any() == true)
				{
					collection.Preview = collection.Preview with { Thumbnail = "" };
				}

				if (!string.IsNullOrEmpty(collection.Hls720)) collection.Hls720 = collection.Hls720;
				if (!string.IsNullOrEmpty(collection.Hls1080)) collection.Hls1080 = collection.Hls1080;
				if (!string.IsNullOrEmpty(collection.Hls480)) collection.Hls480 = collection.Hls480;
			}

			//reorder episodes from zero
			var orderedEpisodes = episodes.OrderBy(a => a.SortOrder);
			var iterator = 0;
			foreach (var orderedEpisode in orderedEpisodes)
			{
				orderedEpisode.SortOrder = iterator;
				iterator++;
			}
		}

		private static async Task SaveLoadedItemsToFiles(
			string folderToSaveCacheFiles,
			List<ReleaseSaveModel> result,
			List<ReleaseTorrentsSaveModel> resultTorrents,
			List<ReleaseSaveEpisodeModel> resultVideos,
			long lastTimestamp)
		{

			var extension = ".cache";

			var countReleaseFiles = await SaveReleasesAsFewFiles(folderToSaveCacheFiles, result, extension);

			var torrentPath = Path.Combine(folderToSaveCacheFiles, $"torrents{extension}");
			Console.WriteLine($"Saving torrents to file {Path.GetFullPath(torrentPath)} items");
			await File.WriteAllTextAsync(torrentPath, SerializeToJson(resultTorrents));

			var countEpisodeFiles = await SaveEpisodesAsFewFiles(folderToSaveCacheFiles, resultVideos, extension);

			var metadataPath = Path.Combine(folderToSaveCacheFiles, "metadata");
			Console.WriteLine($"Saving metadata to file {Path.GetFullPath(metadataPath)} items");
			await File.WriteAllTextAsync(
				metadataPath,
				SerializeToJson(
					new MetadataModel
					{
						LastReleaseTimeStamp = lastTimestamp,
						CountEpisodes = countEpisodeFiles,
						CountReleases = countReleaseFiles
					}
				)
			);
		}

		static async Task<int> SaveEpisodesAsFewFiles(string folderToSaveCacheFiles, List<ReleaseSaveEpisodeModel> allEpisodes, string extension)
		{
			var countInPart = 200;
			var partsCount = (allEpisodes.Count() / countInPart) + 1;
			for (var i = 0; i < partsCount; i++)
			{
				var episodesPath = Path.Combine(folderToSaveCacheFiles, $"episodes{i}{extension}");
				Console.WriteLine($"Saving episodes to file {Path.GetFullPath(episodesPath)} items");

				var items = allEpisodes.Skip(i * countInPart).Take(countInPart).ToList();
				if (items.Any()) await File.WriteAllTextAsync(episodesPath, SerializeToJson(items));
			}

			return partsCount;
		}

		static async Task<int> SaveReleasesAsFewFiles(string folderToSaveCacheFiles, List<ReleaseSaveModel> allReleases, string extension)
		{
			var countInPart = 300;
			var partsCount = (allReleases.Count() / countInPart) + 1;
			for (var i = 0; i < partsCount; i++)
			{
				var episodesPath = Path.Combine(folderToSaveCacheFiles, $"releases{i}{extension}");
				Console.WriteLine($"Saving releases to file {Path.GetFullPath(episodesPath)} items");

				var items = allReleases.Skip(i * countInPart).Take(countInPart).ToList();
				if (items.Any()) await File.WriteAllTextAsync(episodesPath, SerializeToJson(items));
			}

			return partsCount;
		}

		internal static async Task<IResult> AddFavorites(IHttpClientFactory clientFactory, HttpContext context, IEnumerable<int> ids)
		{
			if (!ids.Any()) return Results.Ok();

			var httpClient = clientFactory.CreateClient();
			httpClient.Timeout = TimeSpan.FromSeconds(5);

			var token = context.Request.Cookies?.FirstOrDefault(a => a.Key == Authorize.CookieName).Value ?? null;
			if (token != null)
			{
				await OriginalApiMaker.AddUserFavorites(httpClient, token, ids);
				await Releases.AddToFavorites(ids, isLocal: false);
				return Results.Ok();
			}
			else
			{
				//TODO: save to local favorites
			}

			return Results.NotFound();
		}

		internal static async Task<IResult> RemoveFavorites(IHttpClientFactory clientFactory, HttpContext context, IEnumerable<int> ids)
		{
			if (!ids.Any()) return Results.Ok();

			var httpClient = clientFactory.CreateClient();
			httpClient.Timeout = TimeSpan.FromSeconds(5);

			var token = context.Request.Cookies?.FirstOrDefault(a => a.Key == Authorize.CookieName).Value ?? null;
			if (token != null)
			{
				await OriginalApiMaker.DeleteUserFavorites(httpClient, token, ids);
				await Releases.RemoveFromFavorites(ids, isLocal: false);
				return Results.Ok();
			}
			else
			{
				//TODO: remove from local favorites
			}

			return Results.NotFound();
		}

		internal static async Task<IResult> AddSeens(IHttpClientFactory clientFactory, HttpContext context, string[] ids)
		{
			if (!ids.Any()) return Results.Ok();

			var token = context.Request.Cookies?.FirstOrDefault(a => a.Key == Authorize.CookieName).Value ?? null;
			if (token == null) return Results.Unauthorized();

			var httpClient = clientFactory.CreateClient();
			httpClient.Timeout = TimeSpan.FromSeconds(5);

			await OriginalApiMaker.SetUserSeenMarks(httpClient, token, ids, true);
			await Releases.AddSeenMarksToMemory(ids);

			return Results.Ok();
		}

		internal static async Task<IResult> RemoveSeens(IHttpClientFactory clientFactory, HttpContext context, string[] ids)
		{
			if (!ids.Any()) return Results.Ok();

			var token = context.Request.Cookies?.FirstOrDefault(a => a.Key == Authorize.CookieName).Value ?? null;
			if (token == null) return Results.Unauthorized();

			var httpClient = clientFactory.CreateClient();
			httpClient.Timeout = TimeSpan.FromSeconds(5);

			await OriginalApiMaker.SetUserSeenMarks(httpClient, token, ids, false);
			await Releases.RemoveSeenMarksToMemory(ids);

			return Results.Ok();
		}

	}

}