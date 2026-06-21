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

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapGet("/sync/full", ([FromServices] IHttpClientFactory clientFactory) => Full(clientFactory));
			app.MapGet("/sync/user", ([FromServices] IHttpClientFactory clientFactory, HttpContext context) => User(clientFactory, context));
		}

		public static async Task<IResult> Full(IHttpClientFactory clientFactory)
		{
			var httpClient = clientFactory.CreateClient();
			httpClient.Timeout = TimeSpan.FromSeconds(20);
			var cacheFolder = GlobalConfig.PathToCache();

			if (IsEmptyTypes(cacheFolder)) await SaveTypes(httpClient, cacheFolder);

			await SaveFullReleases(httpClient, cacheFolder);

			return Results.Ok();
		}

		public static async Task<IResult> User(IHttpClientFactory clientFactory, HttpContext context)
		{
			var token = context.Request.Cookies?.FirstOrDefault(a => a.Key == "uft").Value ?? null;
			if (token == null) return Results.Unauthorized();

			var httpClient = clientFactory.CreateClient();
			httpClient.Timeout = TimeSpan.FromSeconds(20);

			var userData = await OriginalApiMaker.GetUserData(httpClient, token);
			var favorites = await OriginalApiMaker.GetUserFavorites(httpClient, token);
			var seens = await OriginalApiMaker.GetUserSeens(httpClient, token);


			return Results.NoContent();
		}

		public static bool IsEmptyTypes(string folderToSaveCacheFiles) => !File.Exists(Path.Combine(folderToSaveCacheFiles, "types.cache"));

		static public async Task SaveFullReleases(HttpClient httpClient, string folderToSaveCacheFiles)
		{
			var totalPages = 300;

			var allReleases = new List<ReleaseDataFullModel>();

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

				if (i % 2 == 0) await Task.Delay(500);
			}

			if (!allReleases.Any()) return;

			allReleases = allReleases
				.Where(a => a.Episodes.Any() || a.Torrents.Any()) // stay only there where have episodes or torrents
				.ToList();

			var lastTimestamp = DateTimeOffset.Parse(allReleases.Where(a => a.FreshAt != null).OrderByDescending(a => a.FreshAt).First().FreshAt).ToUnixTimeSeconds();

			var types = await ReadTypes(folderToSaveCacheFiles);

			var result = new List<ReleaseSaveModel>();
			var resultTorrents = new List<ReleaseTorrentSaveModel>();
			var resultVideos = new List<ReleaseSaveEpisodeModel>();

			foreach (var fullRelease in allReleases.OrderByDescending(a => a.FreshAt))
			{
				//release
				result.Add(new ReleaseSaveModel
				{
					Id = fullRelease.Id,
					Announce = fullRelease.Notification ?? "",
					Code = fullRelease.Alias,
					CountVideos = fullRelease.Episodes?.Count() ?? 0,
					CountTorrents = fullRelease.Torrents?.Count() ?? 0,
					Description = fullRelease.Description ?? "",
					Timestamp = ParseDateTimeOffset(fullRelease.FreshAt),
					OriginalName = fullRelease.Name.English,
					Title = fullRelease.Name.Main,
					Rating = fullRelease.AddedInUsersFavorites ?? 0,
					Year = fullRelease.Year,
					Season = types.Seasons.FirstOrDefault(a => a.Value == fullRelease.Season.Value)?.Description ?? "Не указано",
					Status = fullRelease.IsInProduction ? "Сейчас в озвучке" : "Озвучка завершена",
					Series = fullRelease.EpisodesAreUnknown ? -1 : fullRelease.EpisodesTotal ?? 0,
					Poster = fullRelease.Poster?.Src ?? "",
					Type = types.Types.FirstOrDefault(a => a.Value == fullRelease.Type.Value)?.Description ?? fullRelease.Type.Value,
					Genres = fullRelease.Genres.Select(a => types.Genres.FirstOrDefault(b => b.Id == a.Id)?.Name ?? "").Where(a => !string.IsNullOrEmpty(a)),
					IsOngoing = fullRelease.IsOngoing,
					AgeRating = types.AgeRatings.FirstOrDefault(a => a.Value == fullRelease.AgeRating.Value)?.Description ?? fullRelease.AgeRating.Value,
					Voices = fullRelease.Members != null ? fullRelease.Members.Where(a => a.Role.Value == "voicing").Select(a => a.Nickname) : [],
					Team = fullRelease.Members != null ? fullRelease.Members.OrderByDescending(a => a.Role.Value).Select(a => a.Nickname) : [],
					PublishDay = fullRelease.PublishDay?.Value
				});

				// torrents
				if (fullRelease.Torrents?.Any() == true)
				{
					foreach (var torrent in fullRelease.Torrents)
					{
						resultTorrents.Add(
							new ReleaseTorrentSaveModel
							{
								Id = torrent.Id,
								Codec = torrent.Codec,
								Description = torrent.Description,
								Filename = torrent.Filename,
								Hash = torrent.Hash,
								Magnet = torrent.Magnet,
								Quality = torrent.Quality,
								Type = torrent.Type,
								Size = torrent.Size,
								ReleaseId = fullRelease.Id,
								Seeders = torrent.Seeders,
								Time = ParseDateTimeOffset(torrent.UpdatedAt)
							}
						);
					}
				}

				// map episodes
				if (fullRelease.Episodes?.Any() == true)
				{
					RemapEpisodes(fullRelease.Episodes);
					resultVideos.Add(new ReleaseSaveEpisodeModel { ReleaseId = fullRelease.Id, Items = fullRelease.Episodes });
				}
			}


			await SaveLoadedItemsToFiles(folderToSaveCacheFiles, result, resultTorrents, resultVideos, lastTimestamp);
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

		static long ParseDateTimeOffset(string value)
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
			List<ReleaseTorrentSaveModel> resultTorrents,
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

	}

}