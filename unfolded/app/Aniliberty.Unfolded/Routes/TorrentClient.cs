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

		public static void Initialize()
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

			// delete torrent directory to prevent storing redundant files
			if (Directory.Exists(m_torrentsPath)) Directory.Delete(m_torrentsPath);
		}

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapPost("/torrent/checkfolder", ([FromBody] string path) => CheckFolder(path));
			app.MapPost("/torrent/download", ([FromServices] IHttpClientFactory clientFactory, [FromQuery] int releaseId, string hash) => Download(clientFactory, releaseId, hash));
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

		public static async Task<IResult> Download(IHttpClientFactory clientFactory, int releaseId, string hash)
		{
			if (m_clientEngine == null) return Results.NoContent();

			var torrent = Releases.GetReleaseTorrent(releaseId, hash);
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
				var torrentFile = await Torrent.LoadAsync(httpClient, new Uri(m_torrentsPath), m_torrentsPath);
				torrentManager = await m_clientEngine.AddAsync(torrentFile, m_downloadPath);
			}

			await torrentManager.StartAsync();

			return Results.Ok();
		}
	}

	/*
        private static string GetDownloadStatus ( TorrentManager manager ) {
            var managerModel = m_TorrentManagers.Values.FirstOrDefault ( a => a.Manager == manager );
            if ( managerModel == null ) return "";

            if ( manager.Files.All ( a => a.BitField.AllTrue ) ) {
                var model = new StatusModel { Path = managerModel.DownloadPath, All = true, Id = managerModel.Identifier };
                return "ds:" + JsonSerializer.Serialize ( model, TorrentStreamSerializerContext.Default.StatusModel );
            }

            return "";
        }

        private static ReadOnlyMemory<byte> GetDownloadStatus () {
            var keys = m_TorrentManagers.Keys;
            var result = new List<StatusModel> ();

            foreach ( var managerKey in keys ) {
                if ( m_TorrentManagers.TryGetValue ( managerKey, out var managerModel ) ) {
                    if ( managerModel.Manager == null ) continue;

                    var manager = managerModel.Manager;
                    if ( manager.Files.All ( a => a.BitField.AllTrue ) ) {
                        result.Add ( new StatusModel { Path = managerKey, All = true, Id = managerModel.Identifier } );
                        continue;
                    }
                    var model = new StatusModel { Path = managerKey, All = false, Id = managerModel.Identifier };
                    var index = 0;
                    foreach ( var file in manager.Files ) {
                        model.Files.Add ( index, Convert.ToInt32 ( file.BitField.PercentComplete ) );
                        index++;
                    }
                    result.Add ( model );
                }
            }

            return Encoding.UTF8.GetBytes ( "ds:" + JsonSerializer.Serialize ( result.AsEnumerable (), TorrentStreamSerializerContext.Default.IEnumerableStatusModel ) ).AsMemory ();
        }

        public static string GetTorrentsJson () {
            var managers = m_TorrentManagers.Values;

            var result = new List<FullManagerModel> ();

            foreach ( var manager in managers ) {
                if ( manager.Manager == null ) continue;

                result.Add (
                    new FullManagerModel {
                        Identifier = manager.Identifier,
                        DownloadPath = manager.DownloadPath,
                        AllDownloaded = manager.Manager.Bitfield.PercentComplete >= 100,
                        Files = manager.Manager.Files
                            .Select (
                                a => new TorrentFileModel {
                                    IsDownloaded = a.BitField.PercentComplete >= 100,
                                    PercentComplete = Convert.ToInt32 ( a.BitField.PercentComplete ),
                                    DownloadedPath = a.DownloadCompleteFullPath,
                                    Size = a.Length,
                                }
                            )
                            .OrderBy ( a => a.DownloadedPath )
                            .ToList ()
                    }
                );
            }

            return JsonSerializer.Serialize ( result.AsEnumerable (), TorrentStreamSerializerContext.Default.IEnumerableFullManagerModel );
        }


        public static async Task GetTorrents ( HttpContext context ) {
            if ( m_TorrentManagers.IsEmpty ) {
                await context.Response.WriteAsync ( "[]" );
                return;
            }

            var managers = m_TorrentManagers.Values;

            var result = new List<FullManagerModel> ();

            foreach ( var manager in managers ) {
                if ( manager.Manager == null ) continue;

                result.Add (
                    new FullManagerModel {
                        Identifier = manager.Identifier,
                        DownloadPath = manager.DownloadPath,
                        AllDownloaded = manager.Manager.Bitfield.PercentComplete >= 100,
                        Files = manager.Manager.Files
                            .Select (
                                a => new TorrentFileModel {
                                    IsDownloaded = a.BitField.PercentComplete >= 100,
                                    PercentComplete = Convert.ToInt32 ( a.BitField.PercentComplete ),
                                    DownloadedPath = a.DownloadCompleteFullPath,
                                    Size = a.Length,
                                }
                            )
                            .OrderBy ( a => a.DownloadedPath )
                            .ToList ()
                    }
                );
            }

            await context.Response.WriteAsJsonAsync ( result.AsEnumerable (), typeof ( IEnumerable<FullManagerModel> ), TorrentStreamSerializerContext.Default );
        }

        private static string ConvertToReadableSize ( long count, bool bytesSeconds = false ) {
            if ( count < 0 ) return "";

            string[] suffixes;
            if ( bytesSeconds ) {
                suffixes = ["B/s", "KiB/s", "MiB/s", "GiB/s", "TiB/s"];
            } else {
                suffixes = ["B", "KiB", "MiB", "GiB", "TiB"];
            }
            int suffixIndex = 0;

            double size = count;

            while ( size >= 1024 && suffixIndex < suffixes.Length - 1 ) {
                size /= 1024;
                suffixIndex++;
            }

            return $"{size:0.##} {suffixes[suffixIndex]}";
        }

        private static string GetTorrentState ( TorrentState state ) {
            return state switch {
                TorrentState.Error => "Error",
                TorrentState.Starting => "Started",
                TorrentState.Stopped => "Stopped",
                TorrentState.Stopping => "Stopping",
                TorrentState.Seeding => "Seeding",
                TorrentState.Paused => "Pause",
                TorrentState.HashingPaused => "Hash Pause",
                TorrentState.Downloading => "Download",
                TorrentState.Metadata => "Metadata",
                TorrentState.FetchingHashes => "Fetch Hash",
                TorrentState.Hashing => "Hashing",
                _ => ""
            };
        }

        private static string GetRemainingSize ( ITorrentManagerFile file ) {
            var percents = file.BitField.PercentComplete;
            if ( percents == 100 ) return ConvertToReadableSize ( 0 );
            if ( percents == 0 ) percents = 0.01;
            var number = Convert.ToInt64 ( ( percents / 100 ) * file.Length );
            return ConvertToReadableSize ( file.Length - number );
        }

        public static async Task<string> GetTorrentsAsJson () {
            if ( m_TorrentManagers.IsEmpty ) return "[]";

            var managers = m_TorrentManagers.Values;

            var result = new List<DesktopManagerModel> ();

            foreach ( var manager in managers ) {
                if ( manager.Manager == null ) continue;

                var files = manager.Manager.Files
                    .Select ( a => a.Length )
                    .ToArray ();
                var filesSum = files.Any () ? files.Sum () : 0;
                var peers = await manager.Manager.GetPeersAsync ();
                var count = peers.Count ();
                var torrent = new DesktopManagerModel {
                    Identifier = manager.Identifier,
                    DownloadPath = manager.DownloadPath,
                    AllDownloaded = manager.Manager.Bitfield.PercentComplete >= 100,
                    Percent = Convert.ToInt32 ( Math.Round ( manager.Manager.Bitfield.PercentComplete ) ),
                    Size = ConvertToReadableSize ( filesSum ),
                    TorrentName = manager.Manager.Name,
                    Peers = manager.Manager.Peers.Available,
                    Seeds = manager.Manager.Peers.Seeds,
                    DownloadSpeed = ConvertToReadableSize ( manager.Manager.Monitor.DownloadRate, bytesSeconds: true ),
                    UploadSpeed = ConvertToReadableSize ( manager.Manager.Monitor.UploadRate, bytesSeconds: true ),
                    Status = GetTorrentState ( manager.Manager.State )
                };
                torrent = torrent with {
                    Files = manager.Manager.Files
                        .Select (
                            a => new DesktopTorrentFileModel {
                                Identifier = torrent.Unique + "_" + a.FullPath,
                                IsDownloaded = a.BitField.PercentComplete >= 100,
                                PercentComplete = Convert.ToInt32 ( a.BitField.PercentComplete ),
                                DownloadedPath = a.DownloadCompleteFullPath,
                                Name = a.Path,
                                Percent = Convert.ToInt32 ( Math.Round ( a.BitField.PercentComplete ) ),
                                Priority = GetPriority ( a.Priority ),
                                Size = ConvertToReadableSize ( a.Length ),
                                Remaining = GetRemainingSize ( a )
                            }
                        )
                        .OrderBy ( a => a.DownloadedPath )
                        .ToList (),
                    TorrentPeers = peers
                        .Select (
                            a => new DesktopManagerPeerModel {
                                Identifier = torrent.Unique + "_" + a.PeerID.Text,
                                Percent = Convert.ToInt32 ( Math.Round ( a.BitField.PercentComplete ) ),
                                Address = a.Uri.Host,
                                Port = a.Uri.Port,
                                Client = a.ClientApp.Client.ToString (),
                                DownloadSpeed = ConvertToReadableSize ( a.Monitor.DownloadRate, bytesSeconds: true ),
                                UploadSpeed = ConvertToReadableSize ( a.Monitor.UploadRate, bytesSeconds: true ),
                            }
                        )
                        .ToList ()
                };

                result.Add ( torrent );
            }

            return JsonSerializer.Serialize ( result.AsEnumerable (), TorrentStreamSerializerContext.Default.IEnumerableDesktopManagerModel );
        }

        private static string GetPriority ( Priority priority ) {
            return priority switch {
                Priority.High => "High",
                Priority.Highest => "Highest",
                Priority.Normal => "Normal",
                Priority.Low => "Low",
                Priority.Lowest => "Lowest",
                Priority.Immediate => "Immediate",
                Priority.DoNotDownload => "Idle",
                _ => ""
            };
        }

	 */
}
