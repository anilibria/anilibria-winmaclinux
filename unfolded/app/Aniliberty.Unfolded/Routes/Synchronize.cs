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
			app.MapGet("/sync/full", ([FromQuery] string path, [FromServices] IHttpClientFactory clientFactory) => Full(path, clientFactory));
			//app.MapGet("/videoproxy/part", ([FromQuery] string path) => VideoPart(path));
		}

		public static async Task<IResult> Full(string path, IHttpClientFactory clientFactory)
		{
			if (string.IsNullOrEmpty(path)) return Results.BadRequest();

			var httpClient = clientFactory.CreateClient();

			await SaveFullReleases(httpClient, "");

			return Results.Ok();
		}

		static public async Task SaveFullReleases(HttpClient httpClient, string folderToSaveCacheFiles)
		{
			var types = await ReadTypes(folderToSaveCacheFiles);
			var metadata = await ReadMetadata(folderToSaveCacheFiles);
			if (metadata == null) return;

			var totalPages = 300;

			var allReleases = new List<ReleaseDataFullModel>();

			for (var i = 1; i < 300; i++)
			{
				Console.WriteLine($"Try to get page {i}");
				var page = await OriginalApiMaker.GetPageIds(i, httpClient, 50);
				if (totalPages == 300)
				{
					totalPages = page.Meta.Pagination.TotalPages;
					Console.WriteLine("Total pages: " + totalPages);
				}

				if (!page.Data.Any()) break;

				var ids = page.Data.Select(a => a.Id).ToList();

				allReleases.AddRange((await OriginalApiMaker.GetReleasesInnerCollections(httpClient, ids)).Data);

				if (i % 2 == 0) await Task.Delay(500);
			}

			if (!allReleases.Any()) return;

			var lastTimestamp = DateTimeOffset.Parse(allReleases.Where(a => a.FreshAt != null).OrderByDescending(a => a.FreshAt).First().FreshAt).ToUnixTimeSeconds();

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
					Description = fullRelease.Description,
					Timestamp = ParseDateTimeOffset(fullRelease.FreshAt),
					OriginalName = fullRelease.Name.English,
					Title = fullRelease.Name.Main,
					Rating = fullRelease.AddedInUsersFavorites ?? 0,
					Year = fullRelease.Year.ToString(),
					Season = types.Seasons.FirstOrDefault(a => a.Value == fullRelease.Season.Value)?.Description ?? "Не указано",
					Status = fullRelease.IsInProduction ? "Сейчас в озвучке" : "Озвучка завершена",
					Series = fullRelease.EpisodesAreUnknown ? "?" : $"({fullRelease.EpisodesTotal ?? 0})",
					Poster = fullRelease.Poster.Src,
					Type = types.Types.FirstOrDefault(a => a.Value == fullRelease.Type.Value)?.Description ?? fullRelease.Type.Value,
					Genres = string.Join(", ", fullRelease.Genres.Select(a => types.Genres.FirstOrDefault(b => b.Id == a.Id)?.Name ?? "").Where(a => !string.IsNullOrEmpty(a))),
					IsOngoing = fullRelease.IsOngoing,
					AgeRating = types.AgeRatings.FirstOrDefault(a => a.Value == fullRelease.AgeRating.Value)?.Description ?? fullRelease.AgeRating.Value,
					Voices = fullRelease.Members != null ? string.Join(", ", fullRelease.Members.Where(a => a.Role.Value == "voicing").Select(a => a.Nickname)) : "",
					Team = fullRelease.Members != null ? string.Join(", ", fullRelease.Members.OrderByDescending(a => a.Role.Value).Select(a => a.Nickname)) : ""
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
			var pathToTypes = Path.Combine(folderToSaveCacheFiles, "types.json");
			if (!File.Exists(pathToTypes))
			{
				var errorMessage = $"File types.json not found by path {Path.GetFullPath(pathToTypes)}. You need synchronize types, please add -types or -all parameters to command!";
				Console.WriteLine(errorMessage);
				throw new Exception(errorMessage);
			}
			var types = DeserializeFromJson<TypesResultModel>(await File.ReadAllTextAsync(pathToTypes));
			if (types == null)
			{
				var errorMessage = $"Content of types.json is corrupt. You need synchronize types, please add -types or -all parameters to command!";
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

		//fix domain and language issues
		static string RemakeDomain(string url) => url.Replace("cache.libria.fun", "cache-rfn.libria.fun").Replace("countryIso=US", "countryIso=RU");

		static void RemapEpisodes(IEnumerable<ReleaseEpisodeModel> episodes)
		{
			foreach (var collection in episodes)
			{
				if (collection.Preview?.Thumbnail?.Any() == true)
				{
					collection.Preview = collection.Preview with { Thumbnail = "" };
				}

				if (!string.IsNullOrEmpty(collection.Hls720)) collection.Hls720 = RemakeDomain(collection.Hls720);
				if (!string.IsNullOrEmpty(collection.Hls1080)) collection.Hls1080 = RemakeDomain(collection.Hls1080);
				if (!string.IsNullOrEmpty(collection.Hls480)) collection.Hls480 = RemakeDomain(collection.Hls480);
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