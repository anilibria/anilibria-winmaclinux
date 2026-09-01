using Aniliberty.Unfolded.Configuration;
using Aniliberty.Unfolded.Helpers;
using Aniliberty.Unfolded.Models.Releases;
using Aniliberty.Unfolded.Models.Torrents;
using Microsoft.AspNetCore.Mvc;
using MonoTorrent;
using MonoTorrent.Client;
using System.Net;
using System.Threading.Channels;

namespace Aniliberty.Unfolded.Routes
{

	public class TorrentBackgroundService : BackgroundService
	{

		private static ClientEngine? m_clientEngine;

		private string m_downloadPath = "";

		private static string m_torrentsPath = "";

		public static ClientEngine? ClientEngine => m_clientEngine;

		public static string TorrentsPath => m_torrentsPath;

		private readonly IHttpClientFactory m_httpClientFactory;

		public TorrentBackgroundService(IHttpClientFactory httpClientFactory)
		{
			m_httpClientFactory = httpClientFactory;

			if (!Settings.Model.Torrent.UseInternalTorrectClient) return;

			Console.WriteLine("Inner torrent client starting...");

			int torrentPort = 63245;

			var settings = new EngineSettingsBuilder
			{
				AutoSaveLoadFastResume = true,
				AutoSaveLoadMagnetLinkMetadata = true,
				AutoSaveLoadDhtCache = true,
				AllowPortForwarding = true,
				CacheDirectory = Settings.Model.Torrent.PathToDownloads,
				MaximumConnections = 40,
				MaximumUploadRate = 50 * 1024,
				MaximumDownloadRate = 1024 * 1024, // 1Mb/s
				ListenEndPoints = new Dictionary<string, IPEndPoint>
				{
					["ipv4"] = new IPEndPoint(IPAddress.Any, torrentPort),
					["ipv6"] = new IPEndPoint(IPAddress.IPv6Any, torrentPort),
				},
				AllowLocalPeerDiscovery = true,
			}.ToSettings();
			m_downloadPath = Path.Combine(Settings.Model.Torrent.PathToDownloads, "Downloads");
			m_torrentsPath = Path.Combine(Settings.Model.Torrent.PathToDownloads, "Torrents");
			// delete torrent directory to prevent storing redundant files
			if (Directory.Exists(m_torrentsPath)) Directory.Delete(m_torrentsPath);

			m_clientEngine = new ClientEngine(settings);
			Console.WriteLine("Inner torrent client started!");
			Console.WriteLine("Torrent port: " + torrentPort);
			Console.WriteLine("Torrent folder: " + Settings.Model.Torrent.PathToDownloads);
		}

		protected override async Task ExecuteAsync(CancellationToken stoppingToken)
		{
			if (m_clientEngine is null) return;

			//restore torrents
			foreach (var item in TorrentClient.Cache.Items)
			{
				if (File.Exists(item.MetadataPath))
				{
					var manager = await m_clientEngine.AddAsync(item.MetadataPath, m_downloadPath);
					manager.TorrentStateChanged += async (_, e) =>
					{
						if (e.TorrentManager?.State == TorrentState.Seeding) await WebSocketHub.SendMessage("torrent", "enddownload-" + item.ReleaseId);
					};
					manager.PeerConnected += (o, e) =>
					{
						Console.WriteLine($"Connected to peer {item.ReleaseId}: {e.Peer.Uri}");
					};
				}
			}
			if (TorrentClient.Cache.Items.Any()) await m_clientEngine.StartAllAsync();

			while (!stoppingToken.IsCancellationRequested)
			{
				try
				{
					var message = await TorrentClient.ServiceChannel.Reader.ReadAsync(stoppingToken);
					if (message.StartsWith("download"))
					{
						var releaseId = Convert.ToInt32(message.Substring("download:".Length));
						await Download(releaseId);
					}
					if (message.StartsWith("delete-files"))
					{
						var releaseIds = message.Substring("delete-files:".Length).Split(",").Select(a => Convert.ToInt32(a));
					}
					if (message.StartsWith("delete:"))
					{
						var releaseIds = message.Substring("delete:".Length).Split(",").Select(a => Convert.ToInt32(a));
					}
					if (message.StartsWith("refresh"))
					{
						var releaseIds = message.Substring("refresh:".Length).Split(",").Select(a => Convert.ToInt32(a));
						await RefreshTorrents(releaseIds);
					}
					if (message == "status")
					{
						foreach (var torrent in m_clientEngine.Torrents.Where(a => a.Progress < 100))
						{
							Console.WriteLine($"Download Progress: {torrent.Progress:0.00}%, Speed: {torrent.Monitor.DownloadRate / 1024.0:0.00} KB/s, Peers: {torrent.OpenConnections}");
						}
					}
				}
				catch(Exception ex)
				{
					Console.WriteLine("TorentClient loop: " + ex.Message);
				}
			}
		}

		public override async Task StopAsync(CancellationToken cancellationToken)
		{
			if (m_clientEngine == null) return;
			await m_clientEngine.StopAllAsync();
		}

		public async Task RefreshTorrents(IEnumerable<int> ids)
		{
			if (m_clientEngine is null) return;

			Console.WriteLine("Try to refresh torrents");

			foreach (var id in ids)
			{
				try
				{
					var torrent = Releases.GetReleaseTorrentByCodec(id, Settings.Model.Torrent.CodecPrefference);
					if (torrent is null) continue;

					//remove current torrent if it exists
					var removed = await RemoveTorrent(id, false, true);

					if (removed)
					{
						var magnetLink = MagnetLink.Parse(torrent.Magnet);
						var torrentManager = await m_clientEngine.AddAsync(magnetLink, m_downloadPath);

						await torrentManager.StartAsync();
						await torrentManager.WaitForMetadataAsync();
						RegisterTorrentManager(torrentManager, id, torrent);
					}
				}
				catch (Exception ex)
				{
					Console.WriteLine($"Refresh torrents error for release {id}: {ex.Message}");
				}
			}

			await TorrentClient.SaveTorrentCache();

			Console.WriteLine("Refresh torrents completed");
		}

		private static void RegisterTorrentManager(TorrentManager torrentManager, int releaseId, ReleaseTorrentSaveModel torrent)
		{
			torrentManager.TorrentStateChanged += async (_, e) =>
			{
				if (e.TorrentManager?.State == TorrentState.Seeding) await WebSocketHub.SendMessage("torrent", "enddownload-" + releaseId);
			};
			var codec = torrent.Codec ?? "";
			var filesCount = torrentManager.Files.Count;
			var folder = torrentManager.ContainingDirectory;
			TorrentClient.Cache.Items.Add(new TorrentCacheItem { ReleaseId = releaseId, CountVideos = filesCount, Codec = codec, Path = folder, MetadataPath = torrentManager.MetadataPath });
		}

		public async Task<IResult> Download(int releaseId)
		{
			if (m_clientEngine == null) return Results.NoContent();

			var torrent = Releases.GetReleaseTorrentByCodec(releaseId, Settings.Model.Torrent.CodecPrefference);
			if (torrent == null) return Results.NotFound();

			TorrentManager? torrentManager;
			if (!string.IsNullOrEmpty(torrent.Magnet))
			{
				var magnetLink = MagnetLink.Parse(torrent.Magnet);
				torrentManager = await m_clientEngine.AddAsync(magnetLink, m_downloadPath);
			}
			else
			{
				var httpClient = m_httpClientFactory.CreateClient();
				var torrentFile = await Torrent.LoadAsync(httpClient, new Uri($"https://www.anilibria.top/api/v1/anime/torrents/{torrent.Id}/file"), m_torrentsPath);
				torrentManager = await m_clientEngine.AddAsync(torrentFile, m_downloadPath);
			}

			await torrentManager.StartAsync();
			await WebSocketHub.SendMessage("torrent", "startmeta-" + releaseId);

			using var cts = new CancellationTokenSource(TimeSpan.FromSeconds(60));
			try
			{
				await torrentManager.WaitForMetadataAsync(cts.Token);
				await WebSocketHub.SendMessage("torrent", "startdownload-" + releaseId);
			}
			catch (OperationCanceledException)
			{
				await WebSocketHub.SendMessage("torrent", "failmetadata-" + releaseId);
			}

			// if it was item plus torrent before we remove it
			if (TorrentClient.Cache.Items.Any(a => a.ReleaseId == releaseId))
			{
				var lessSeriesItem = TorrentClient.Cache.Items.First(a => a.ReleaseId == releaseId);
				var lessManager = m_clientEngine.Torrents.FirstOrDefault(a => a.MetadataPath == lessSeriesItem.MetadataPath);
				if (lessManager is not null)
				{
					await lessManager.StopAsync();
					await m_clientEngine.RemoveAsync(lessManager);
				}
				TorrentClient.Cache.Items.Remove(lessSeriesItem);
			}

			RegisterTorrentManager(torrentManager, releaseId, torrent);

			await TorrentClient.SaveTorrentCache();

			return Results.Ok();
		}

		internal async Task<bool> RemoveTorrent(int releaseId, bool removeFiles, bool saveImmediateCache)
		{
			if (m_clientEngine is null) return true;

			var torrent = TorrentClient.Cache.Items.FirstOrDefault(a => a.ReleaseId == releaseId);
			if (torrent == null) return false;

			var manager = m_clientEngine.Torrents.FirstOrDefault(a => a.MetadataPath == torrent.MetadataPath);
			if (manager == null)
			{
				// if we have only record in cache file we just delete these empty item
				TorrentClient.Cache.Items.Remove(torrent);
				return true;
			}

			await manager.StopAsync();
			await m_clientEngine.RemoveAsync(manager);
			if (removeFiles) Directory.Delete(torrent.Path, true);
			TorrentClient.Cache.Items.Remove(torrent);

			if (saveImmediateCache) await TorrentClient.SaveTorrentCache();

			return true;
		}

		internal async Task<IResult> Remove(int releaseId, bool removeFiles)
		{
			if (m_clientEngine == null) return Results.NoContent();

			await RemoveTorrent(releaseId, removeFiles, true);

			return Results.Ok();
		}

		internal async Task<IResult> RemoveMulti([FromBody] IEnumerable<int> releaseIds, [FromQuery] bool removeFiles)
		{
			if (m_clientEngine == null) return Results.NoContent();

			foreach (var releaseId in releaseIds)
			{
				await RemoveTorrent(releaseId, removeFiles, true);
			}

			return Results.Ok();
		}

	}

	public static class TorrentClient
	{

		private static TorrentCache m_cache = new();

		public static TorrentCache Cache => m_cache;

		private static string m_torrentCacheFile = "";

		private static Channel<string> m_serviceChannel = Channel.CreateBounded<string>(100);

		public static Channel<string> ServiceChannel => m_serviceChannel;

		public static void Initialize()
		{
			m_torrentCacheFile = Path.Combine(Settings.Model.Torrent.PathToDownloads, "torrents.cache");
			LoadTorrentCache();
		}

		public static void LoadTorrentCache()
		{
			if (!File.Exists(m_torrentCacheFile)) return;

			var fullJson = File.ReadAllText(m_torrentCacheFile);
			if (fullJson == null) return;

			var serializedItems = JsonHelpers.DeserializeFromJson<TorrentCache>(fullJson);
			if (serializedItems != null) m_cache = serializedItems;
		}

		public static async Task SaveTorrentCache()
		{
			var json = JsonHelpers.SerializeToJson(m_cache);
			await File.WriteAllTextAsync(m_torrentCacheFile, json);
		}

		internal static async Task RefreshTorrents(IEnumerable<int> ids)
		{
			await m_serviceChannel.Writer.WriteAsync($"refresh:" + string.Join(",", ids));
		}

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapPost("/torrent/checkfolder", ([FromBody] string path) => CheckFolder(path));
			app.MapGet("/torrent/download", ([FromQuery] int releaseId) => m_serviceChannel.Writer.WriteAsync($"download:{releaseId}"));
			app.MapGet("/torrent/openinexternalclient", ([FromServices] IHttpClientFactory clientFactory, [FromQuery] int releaseId, string hash) => OpenInExternalClient(clientFactory, releaseId, hash));
			app.MapGet("/torrent/remove", ([FromQuery] int releaseId, [FromQuery] bool removeFiles) => m_serviceChannel.Writer.WriteAsync($"delete{(removeFiles ? "-files" : "")}:" + releaseId));
			app.MapPost("/torrent/removemulti",
				([FromBody] IEnumerable<int> releaseIds, [FromQuery] bool removeFiles) => m_serviceChannel.Writer.WriteAsync($"delete{(removeFiles ? "-files" : "")}:" + string.Join(",", releaseIds))
			);
			app.MapPost("/torrent/list", ([FromBody] ReleasesListFiltersModel model) => TorrentList(model));
			app.MapGet("/torrent/active", GetActiveTorrents);
			app.MapGet("/torrent/videofile/{releaseId}/{videoIndex}/", GetTorrentVideoFile);
			app.MapGet("/torrent/episodes", Episodes);
		}

		public static IResult CheckFolder(string path)
		{
			var absoluteName = Path.GetFullPath(path);
			var exists = Directory.Exists(absoluteName);
			if (!exists)
			{
				try
				{
					Directory.CreateDirectory(absoluteName);
					exists = true;
				}
				catch
				{
				}
			}

			return Results.Content(exists ? "true" : "false", "application/json");
		}



		internal static async Task<IResult> OpenInExternalClient(IHttpClientFactory clientFactory, int releaseId, string hash)
		{
			var torrent = Releases.GetReleaseTorrentByHash(releaseId, hash);
			if (torrent == null) return Results.NotFound();

			var httpClient = clientFactory.CreateClient();
			try
			{
				var response = await httpClient.GetAsync($"https://www.anilibria.top/api/v1/anime/torrents/{torrent.Id}/file");
				var tempFile = Path.Combine(TorrentBackgroundService.TorrentsPath, Guid.NewGuid().ToString().Replace("-", "") + ".torrent");
				using var savedFile = File.OpenRead(Path.Combine(TorrentBackgroundService.TorrentsPath, tempFile));
				await response.Content.CopyToAsync(savedFile);
				GlobalConfig.OpenPathInSystem(tempFile);
				return Results.Ok();
			}
			catch
			{
				return Results.InternalServerError();
			}
		}

		internal static async Task<IResult> TorrentList(ReleasesListFiltersModel model)
		{
			var releaseIds = m_cache.Items.Select(a => a.ReleaseId).ToHashSet();

			var filteredItems = Releases.FilterReleases(model)
				.Where(a => releaseIds.Contains(a.Id))
				.ToList();

			return Results.Json(filteredItems, AppJsonSerializerContext.Default);
		}

		internal static async Task<IResult> GetActiveTorrents()
		{
			if (TorrentBackgroundService.ClientEngine is null) return Results.Content("[]", "application/json");

			await m_serviceChannel.Writer.WriteAsync("status");
			var result = m_cache.Items
				.Select(
					a => new TorrentCacheDisplayItem
					{
						Codec = a.Codec,
						ReleaseId = a.ReleaseId,
						CountVideos = a.CountVideos,
						CountDownloaded = TorrentBackgroundService.ClientEngine.Torrents.FirstOrDefault(b => b.MetadataPath == a.MetadataPath)?.Files.Count(c => c.BitField.PercentComplete >= 100f) ?? 0,
					}
				)
				.ToDictionary(a => a.ReleaseId);
			return Results.Json(result, AppJsonSerializerContext.Default);
		}

		internal static IResult GetTorrentVideoFile([FromRoute] int releaseId, [FromRoute] int videoIndex)
		{
			if (TorrentBackgroundService.ClientEngine == null) return Results.NoContent();

			var torrent = m_cache.Items.FirstOrDefault(a => a.ReleaseId == releaseId);
			if (torrent == null) return Results.NotFound();

			var manager = TorrentBackgroundService.ClientEngine.Torrents.FirstOrDefault(a => a.MetadataPath == torrent.MetadataPath);
			if (manager == null) return Results.NotFound();

			if (videoIndex >= manager.Files.Count) return Results.NotFound();

			var file = manager.Files.ElementAt(videoIndex);

			var fileName = Path.GetFileName(file.FullPath);
			return Results.File(file.FullPath, "video/x-matroska", fileDownloadName: fileName);
		}

		internal static IResult Episodes(HttpContext httpContext, int releaseId)
		{
			if (TorrentBackgroundService.ClientEngine is null) return Results.NoContent();

			var torrent = m_cache.Items.FirstOrDefault(a => a.ReleaseId == releaseId);
			if (torrent is null) return Results.NotFound();

			var manager = TorrentBackgroundService.ClientEngine.Torrents.FirstOrDefault(a => a.MetadataPath == torrent.MetadataPath);
			if (manager is null) return Results.NotFound();

			var episodes = Releases.GetReleaseEpisodes(releaseId);
			if (episodes is null) return Results.NotFound();

			var host = $"{httpContext.Request.Scheme}://{httpContext.Request.Host}";

			return Results.Json(
				manager.Files
					.OrderBy(a => a.FullPath)
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
					),
				AppJsonSerializerContext.Default
			);
		}

	}

}
