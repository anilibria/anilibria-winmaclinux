using Aniliberty.Unfolded.Configuration;
using Aniliberty.Unfolded.Helpers;
using Aniliberty.Unfolded.Models.Releases;
using Aniliberty.Unfolded.Models.Torrents;
using Microsoft.AspNetCore.Mvc;
using MonoTorrent;
using MonoTorrent.Client;

namespace Aniliberty.Unfolded.Routes
{

	public static class TorrentClient
	{

		private static ClientEngine? m_clientEngine;

		private static string m_downloadPath = "";

		private static string m_torrentsPath = "";

		private static string m_torrentCacheFile = "";

		private static TorrentCache m_cache = new();

		public static async Task Initialize()
		{
			var settings = new EngineSettingsBuilder
			{
				AutoSaveLoadFastResume = true,
				AutoSaveLoadMagnetLinkMetadata = true,
				AutoSaveLoadDhtCache = true,
				AllowPortForwarding = true,
				CacheDirectory = Settings.Model.Torrent.PathToDownloads
			}.ToSettings();

			m_clientEngine = new ClientEngine(settings);
			m_downloadPath = Path.Combine(Settings.Model.Torrent.PathToDownloads, "Downloads");
			m_torrentsPath = Path.Combine(Settings.Model.Torrent.PathToDownloads, "Torrents");
			m_torrentCacheFile = Path.Combine(Settings.Model.Torrent.PathToDownloads, "torrents.cache");

			// delete torrent directory to prevent storing redundant files
			if (Directory.Exists(m_torrentsPath)) Directory.Delete(m_torrentsPath);

			LoadTorrentCache();
			//restore torrents
			foreach (var item in m_cache.Items)
			{
				if (File.Exists(item.MetadataPath)) await m_clientEngine.AddAsync(item.MetadataPath, m_downloadPath);
			}
			await m_clientEngine.StartAllAsync();
		}

		public static async Task Finilize()
		{
			if (m_clientEngine == null) return;
			await m_clientEngine.StopAllAsync();
		}

		private static void LoadTorrentCache()
		{
			if (!File.Exists(m_torrentCacheFile)) return;

			var fullJson = File.ReadAllText(m_torrentCacheFile);
			if (fullJson == null) return;

			var serializedItems = JsonHelpers.DeserializeFromJson<TorrentCache>(fullJson);
			if (serializedItems != null) m_cache = serializedItems;
		}

		private static async Task SaveTorrentCache()
		{
			var json = JsonHelpers.SerializeToJson(m_cache);
			await File.WriteAllTextAsync(m_torrentCacheFile, json);
		}

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapPost("/torrent/checkfolder", ([FromBody] string path) => CheckFolder(path));
			app.MapGet("/torrent/download", ([FromServices] IHttpClientFactory clientFactory, [FromQuery] int releaseId, string hash) => Download(clientFactory, releaseId));
			app.MapGet("/torrent/openinexternalclient", ([FromServices] IHttpClientFactory clientFactory, [FromQuery] int releaseId, string hash) => OpenInExternalClient(clientFactory, releaseId, hash));
			app.MapGet("/torrent/remove", ([FromQuery] int releaseId, [FromQuery] string description, [FromQuery] bool removeFiles) => Remove(releaseId, removeFiles));
			app.MapGet("/torrent/list", (ReleasesListFiltersModel model) => TorrentList(model));
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

		public static async Task<IResult> Download(IHttpClientFactory clientFactory, int releaseId)
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
				var httpClient = clientFactory.CreateClient();
				var torrentFile = await Torrent.LoadAsync(httpClient, new Uri($"https://www.anilibria.top/api/v1/anime/torrents/{torrent.Id}/file"), m_torrentsPath);
				torrentManager = await m_clientEngine.AddAsync(torrentFile, m_downloadPath);
			}

			await torrentManager.StartAsync();
			await torrentManager.WaitForMetadataAsync();

			var description = $"{(torrent.Quality ?? "")} {(torrent.Codec ?? "")}";
			var filesCount = torrentManager.Files.Count;
			var folder = torrentManager.ContainingDirectory;

			if (!m_cache.Items.Any(a => a.ReleaseId == releaseId && a.Codec == description && a.CountVideos == filesCount && a.Path == folder))
			{
				m_cache.Items.Add(new TorrentCacheItem { ReleaseId = releaseId, CountVideos = filesCount, Codec = description, Path = folder, MetadataPath = torrentManager.MetadataPath });

				var lessSeriesItem = m_cache.Items.FirstOrDefault(a => a.ReleaseId == releaseId && a.Codec == description && a.CountVideos < filesCount);
				if (lessSeriesItem != null)
				{
					var lessManager = m_clientEngine.Torrents.FirstOrDefault(a => a.MetadataPath == lessSeriesItem.MetadataPath);
					if (lessManager is not null) await m_clientEngine.RemoveAsync(lessManager);
					m_cache.Items.Remove(lessSeriesItem);
				}
			}

			await SaveTorrentCache();


			return Results.Ok();
		}

		internal static async Task<IResult> OpenInExternalClient(IHttpClientFactory clientFactory, int releaseId, string hash)
		{
			var torrent = Releases.GetReleaseTorrentByHash(releaseId, hash);
			if (torrent == null) return Results.NotFound();

			var httpClient = clientFactory.CreateClient();
			try
			{
				var response = await httpClient.GetAsync($"https://www.anilibria.top/api/v1/anime/torrents/{torrent.Id}/file");
				var tempFile = Path.Combine(m_torrentsPath, Guid.NewGuid().ToString().Replace("-", "") + ".torrent");
				using var savedFile = File.OpenRead(Path.Combine(m_torrentsPath, tempFile));
				await response.Content.CopyToAsync(savedFile);
				GlobalConfig.OpenPathInSystem(tempFile);
				return Results.Ok();
			}
			catch
			{
				return Results.InternalServerError();
			}
		}

		internal static async Task<IResult> Remove(int releaseId, bool removeFiles)
		{
			if (m_clientEngine == null) return Results.NoContent();

			var torrent = m_cache.Items.FirstOrDefault(a => a.ReleaseId == releaseId);
			if (torrent == null) return Results.NotFound();

			var manager = m_clientEngine.Torrents.FirstOrDefault(a => a.MetadataPath == torrent.MetadataPath);
			if (manager == null) return Results.NotFound();

			await m_clientEngine.RemoveAsync(manager);
			if (removeFiles) Directory.Delete(torrent.Path, true);

			return Results.Ok();
		}

		internal static async Task<IResult> TorrentList(ReleasesListFiltersModel model)
		{
			var releaseIds = m_cache.Items.Select(a => a.ReleaseId).ToHashSet();

			var filteredItems = Releases.FilterReleases(model)
				.Where(a => releaseIds.Contains(a.Id))
				.ToList();

			return Results.Json(filteredItems, AppJsonSerializerContext.Default);
		}

	}

}
