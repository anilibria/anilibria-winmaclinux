using Aniliberty.Unfolded.Models.Releases;
using Microsoft.AspNetCore.Mvc;

namespace Aniliberty.Unfolded.Routes
{

	public static class CinemaHall
	{

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapGet("/cinemahall/episodes", Episodes);
			app.MapPost("/cinemahall/list", FullList);
			app.MapPost("/cinemahall/removeseens", RemoveSeens);
			app.MapPost("/cinemahall/removeall", RemoveAll);
		}

		internal static async Task<IResult> FullList([FromBody] ReleasesListFiltersModel model)
		{
			var releaseIds = AppData.Model.Cinemahall.ToHashSet();

			var filteredItems = Releases.FilterReleases(model)
				.Where(a => releaseIds.Contains(a.Id))
				.ToList();

			return Results.Json(filteredItems, AppJsonSerializerContext.Default);
		}

		internal static async Task<IResult> RemoveSeens()
		{
			var result = new List<int>();

			foreach (var id in AppData.Model.Cinemahall.ToHashSet())
			{
				if (!Releases.IsReleaseSeensByEnd(id)) continue;

				result.Add(id);
			}

			await AppData.RemoveCinemahall(result);

			return Results.Ok();
		}

		private static async Task RemoveAll()
		{
			await AppData.RemoveAllCinemahall();
		}

		internal static IResult Episodes(HttpContext httpContext)
		{
			var useOnlyOnline = Settings.Model.Cinamahall.UseOnlyOnline;
			var host = $"{httpContext.Request.Scheme}://{httpContext.Request.Host}";

			var result = new List<ReleaseDisplayGroupEpisodeModel>();

			foreach (var releaseId in AppData.Model.Cinemahall)
			{
				var release = Releases.GetReleaseById(releaseId);
				if (release is null) continue;
				var episodes = Releases.GetReleaseEpisodes(releaseId);
				if (episodes is null) continue;

				if (!useOnlyOnline && TorrentClient.IsInActiveTorrents(releaseId))
				{
					
					result.Add(
						new ReleaseDisplayGroupEpisodeModel
						{
							ReleaseId = release.Id,
							Name = release.Title,
							Poster = release.Poster,
							Mode = "torrent",
							Episodes = episodes.Items
								.Select(
									(a, index) =>
									{
										var episode = episodes.Items.ElementAt(index);
										var pathToVideoFile = host + $"/torrent/videofile/{releaseId}/{index}/";

										return new ReleaseDisplayEpisodeModel
										{
											Id = episode.Id,
											Name = episode.Name,
											Hls1080 = pathToVideoFile,
											Hls480 = pathToVideoFile,
											Hls720 = pathToVideoFile,
											Ordinal = episode.Ordinal,
											Preview = episode.Poster,
											RutubeId = episode.RutubeId,
											YoutubeId = episode.YoutubeId,
											SortOrder = episode.SortOrder,
											OpeningEnd = episode.OpeningEnd,
											OpeningStart = episode.OpeningStart,
											EndingEnd = episode.EndingEnd,
											EndingStart = episode.EndingStart
										};
									}

								)
								.ToArray()
						}
					);
					continue;
				}

				result.Add(
					new ReleaseDisplayGroupEpisodeModel
					{
						ReleaseId = release.Id,
						Name = release.Title,
						Poster = release.Poster,
						Mode = "online",
						Episodes = episodes.Items
							.Select(episode => new ReleaseDisplayEpisodeModel
							{
								Id = episode.Id,
								Name = episode.Name,
								Hls1080 = episode.Hls1080,
								Hls480 = episode.Hls480,
								Hls720 = episode.Hls720,
								Ordinal = episode.Ordinal,
								Preview = episode.Poster,
								RutubeId = episode.RutubeId,
								YoutubeId = episode.YoutubeId,
								SortOrder = episode.SortOrder,
								OpeningEnd = episode.OpeningEnd,
								OpeningStart = episode.OpeningStart,
								EndingEnd = episode.EndingEnd,
								EndingStart = episode.EndingStart
							}
						)
						.ToArray()
					}
				);
			}

			return Results.Json(result, AppJsonSerializerContext.Default);
		}

	}

}
