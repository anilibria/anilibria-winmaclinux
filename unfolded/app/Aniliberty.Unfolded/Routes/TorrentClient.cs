namespace Aniliberty.Unfolded.Routes
{
	public class TorrentClient
	{
	}

	/*
	 using MonoTorrent.Client;
using MonoTorrent;
using System.Collections.Concurrent;
using MonoTorrent.Streaming;
using System.Runtime.InteropServices;
using System.Net.WebSockets;
using System.Text;
using System.Text.Json;
using TorrentStream.Models;
using TorrentStream.SerializerContexts;
using System.Reflection;
using System.Diagnostics.CodeAnalysis;

namespace TorrentStream {

    public static class StreamProviderHelpers {

        private static PropertyInfo? GetActiveStreamProperty ( [DynamicallyAccessedMembers ( DynamicallyAccessedMemberTypes.NonPublicProperties )] Type type ) {
            return type.GetProperties ( BindingFlags.NonPublic | BindingFlags.Instance ).FirstOrDefault ( a => a.Name == "ActiveStream" );
        }

        private static MethodInfo? GetDisposeMethod ( [DynamicallyAccessedMembers ( DynamicallyAccessedMemberTypes.PublicMethods )] Type type ) {
            return type.GetMethod ( "Dispose" );
        }

        public static void DisposeInnerStream ( StreamProvider streamProvider ) {
            var type = typeof ( StreamProvider );

            PropertyInfo? property = GetActiveStreamProperty ( type );
            if ( property == null ) return;

            object? activeStream = property.GetMethod?.Invoke ( streamProvider, null );
            if ( activeStream == null ) return;

            var activeStreamType = typeof ( Stream );

            var method = GetDisposeMethod ( activeStreamType );
            if ( method == null ) return;

            method.Invoke ( activeStream, null );
        }
    }

    public static class TorrentHandler {

        private static ClientEngine? m_ClientEngine;

        public static void Initialize () {
            var pathToCache = Path.Combine ( GlobalConfiguration.BaseFolder, "cache" );
            Console.WriteLine ( "Path to cache:" + pathToCache );
            var settingBuilder = new EngineSettingsBuilder {
                CacheDirectory = pathToCache
            };
            m_ClientEngine = new ClientEngine ( settingBuilder.ToSettings () );

            DownloadsPath = Path.Combine ( GlobalConfiguration.BaseFolder, "Downloads" );
            StateFilePath = Path.Combine ( GlobalConfiguration.BaseFolder, "statefile" );
            InnerStateFilePath = Path.Combine ( GlobalConfiguration.BaseFolder, "innerstatefile" );
        }

        private static string DownloadsPath = Path.Combine ( GlobalConfiguration.BaseFolder, "Downloads" );

        private static string StateFilePath = Path.Combine ( GlobalConfiguration.BaseFolder, "statefile" );

        private static string InnerStateFilePath = Path.Combine ( GlobalConfiguration.BaseFolder, "innerstatefile" );

        public static readonly HashSet<string> m_DownloadedTorrents = new ();

        public static ConcurrentDictionary<string, ManagerModel> m_TorrentManagers = new ();

        public static readonly ConcurrentDictionary<WebSocket, bool> m_ActiveWebSockets = new ();

        private static async Task<(Stream?, bool)> GetTorrentStream ( string url ) {
            if ( m_DownloadedTorrents.Contains ( url ) ) return (null, true);

            if ( url.StartsWith ( "file://" ) ) {
                var filePath = url.Replace ( "file:///", "" ).Replace ( "file://", "" );
                try {
                    using var file = File.OpenRead ( filePath );
                    var innerStream = new MemoryStream ();
                    await file.CopyToAsync ( innerStream );
                    return (innerStream, true);
                } catch {
                    return (null, false);
                }
            }

            if ( url.StartsWith ( "magnet" ) ) return (null, true);

            try {
                var httpClient = new HttpClient ();
                var downloadedContent = await httpClient.GetStreamAsync ( url );
                var innerStream = new MemoryStream ();
                await downloadedContent.CopyToAsync ( innerStream );
                m_DownloadedTorrents.Add ( url );
                return (innerStream, true);
            } catch {
                return (null, false);
            }
        }

        private static string GetStringValueFromQuery ( string key, HttpContext httpContext ) {
            return httpContext.Request.Query.ContainsKey ( key ) ? httpContext.Request.Query.Where ( a => a.Key == key ).FirstOrDefault ().Value.First () ?? "" : "";
        }

        public static async Task StartDownloadForOnlineStreaming ( HttpContext context ) {
            context.Response.ContentType = "text/plain";
            if ( context.Request.Query.Count != 3 ) {
                context.Response.StatusCode = 204;
                return;
            }

            var fileIndex = GetStringValueFromQuery ( "index", context );
            var torrentPath = GetStringValueFromQuery ( "path", context );
            var identifier = GetStringValueFromQuery ( "id", context );

            var activeFileIndex = Convert.ToInt32 ( fileIndex );

            var (torrentStream, result) = await GetTorrentStream ( torrentPath );

            if ( !result ) {
                context.Response.StatusCode = 400;
                return;
            }

            try {
                var manager = await GetManager ( torrentPath, torrentStream, identifier );
                if ( manager == null ) {
                    context.Response.StatusCode = 404;
                    return;
                }
                var iterator = 0;
                var torrentFiles = manager.Files
                    .OrderBy ( a => a.Path )
                    .ToList ();
                foreach ( var file in torrentFiles ) {
                    await manager.SetFilePriorityAsync ( file, iterator == activeFileIndex ? Priority.High : Priority.DoNotDownload );
                    iterator++;
                }

                var currentFile = torrentFiles.ElementAt ( activeFileIndex );
                var isDownloaded = currentFile.BitField.PercentComplete >= 100;

                if ( !isDownloaded ) {
                    if ( manager.StreamProvider != null ) {
                        StreamProviderHelpers.DisposeInnerStream ( manager.StreamProvider );
                        var httpStream = await manager.StreamProvider.CreateHttpStreamAsync ( currentFile, false );
                        if ( httpStream != null ) {
                            context.Response.StatusCode = 302;
                            context.Response.Headers.Location = httpStream.FullUri;
                        }
                    }
                } else {
                    context.Response.StatusCode = 302;
                    var fileName = Uri.EscapeDataString ( Path.GetFileName ( currentFile.FullPath ) );
                    var filePath = Path.GetDirectoryName ( currentFile.FullPath ) ?? "";
                    var fullPath = Path.Combine ( filePath, fileName );
                    context.Response.Headers.Location = ( RuntimeInformation.IsOSPlatform ( OSPlatform.Windows ) ? "file:///" : "file://" ) + fullPath;
                }
            } catch ( Exception exception ) {
                context.Response.StatusCode = 500;
                await context.Response.WriteAsync ( "Error:" + exception.Message + "\n" + exception.StackTrace );
            }
        }

        private static async Task<TorrentManager?> GetManager ( string torrentPath, Stream? torrentStream, string identifier ) {
            TorrentManager? manager = null;
            if ( m_TorrentManagers.TryGetValue ( torrentPath, out var createdManager ) ) {
                manager = createdManager.Manager ?? throw new Exception ( "Manager is null!" );
            } else {
                var isMagnet = torrentPath.StartsWith ( "magnet" );
                if ( isMagnet ) {
                    var magnetLink = MagnetLink.Parse ( torrentPath );
                    manager = await m_ClientEngine!.AddStreamingAsync ( magnetLink, DownloadsPath );
                }
                if ( torrentStream != null ) {
                    torrentStream.Position = 0;
                    var torrent = await Torrent.LoadAsync ( torrentStream );
                    manager = await m_ClientEngine!.AddStreamingAsync ( torrent, DownloadsPath );
                }

                if ( manager == null ) return null;

                await manager.StartAsync ();
                await manager.WaitForMetadataAsync ();
                m_TorrentManagers.TryAdd (
                    torrentPath,
                    new ManagerModel {
                        DownloadPath = torrentPath,
                        Manager = manager,
                        Identifier = Convert.ToInt32 ( identifier ),
                        MetadataId = manager.InfoHashes.V1OrV2.ToHex ()
                    }
                );
                SendMessageToSocket ( "nt:" + identifier );
                await SaveState ();
            }

            return manager;
        }

        public enum FullDownloadResult { NoError, NotCorrectParameters, NotFound, Error };

        public static async Task<FullDownloadResult> StartFullDownload ( string torrentPath, int identifier ) {
            var (torrentStream, result) = await GetTorrentStream ( torrentPath );

            if ( !result ) return FullDownloadResult.NotCorrectParameters;

            try {
                var manager = await GetManager ( torrentPath, torrentStream, identifier.ToString () );
                if ( manager != null ) {
                    foreach ( var file in manager.Files.Where ( a => a.Priority == Priority.DoNotDownload ) ) {
                        await manager.SetFilePriorityAsync ( file, Priority.Normal );
                    }
                    manager.TorrentStateChanged += ManagerTorrentStateChanged;

                    return FullDownloadResult.NoError;
                }
            } catch {
                return FullDownloadResult.Error;
            }

            return FullDownloadResult.Error;
        }

        public static async Task StartFullDownload ( HttpContext context ) {
            context.Response.ContentType = "text/plain";
            if ( context.Request.Query.Count != 2 ) {
                context.Response.StatusCode = 204;
                return;
            }

            var torrentPath = GetStringValueFromQuery ( "path", context );
            var identifier = GetStringValueFromQuery ( "id", context );

            torrentPath = "magnet:?xt=urn:btih:0bb164fdba0e8ad806d25562fd94c9a09f81b5d1&dn=Kanojo+Okarishimasu+4rd+Season+-+AniLiberty+%5BWEB-DL+1080p+HEVC%5D&xl=3121992505&tr=http://tr.libria.fun:2710/announce&tr=http://retracker.local/announce";

            var result = await StartFullDownload ( torrentPath, Convert.ToInt32 ( identifier ) );

            context.Response.StatusCode = result switch {
                FullDownloadResult.NotCorrectParameters => 400,
                FullDownloadResult.NotFound => 404,
                FullDownloadResult.Error => 500,
                _ => 200,
            };

            if ( result == FullDownloadResult.NoError ) {
                SendMessageToSocket ( "nt:" + identifier );
                await context.Response.WriteAsync ( "Downloading started" );
                return;
            }
        }

        private static void ManagerTorrentStateChanged ( object? sender, TorrentStateChangedEventArgs e ) {
            if ( e.TorrentManager == null ) return;

            if ( e.TorrentManager.State == TorrentState.Seeding ) {
                var message = GetDownloadStatus ( e.TorrentManager );
                if ( !string.IsNullOrEmpty ( message ) ) SendMessageToSocket ( message );
            }

            Task.Run ( SaveState );
        }

        private static string GetDownloadStatus ( TorrentManager manager ) {
            var managerModel = m_TorrentManagers.Values.FirstOrDefault ( a => a.Manager == manager );
            if ( managerModel == null ) return "";

            if ( manager.Files.All ( a => a.BitField.AllTrue ) ) {
                var model = new StatusModel { Path = managerModel.DownloadPath, All = true, Id = managerModel.Identifier };
                return "ds:" + JsonSerializer.Serialize ( model, TorrentStreamSerializerContext.Default.StatusModel );
            }

            return "";
        }

        private static void SendMessageToSocket ( string message ) {
            if ( !m_ActiveWebSockets.Any () ) return;

            var messageInBytes = new ReadOnlyMemory<byte> ( Encoding.UTF8.GetBytes ( message ) );
            foreach ( var socket in m_ActiveWebSockets.Keys ) {
                if ( socket.State != WebSocketState.Open ) continue;
                Task.Run ( async () => await socket.SendAsync ( messageInBytes, WebSocketMessageType.Text, true, CancellationToken.None ) );
            }
        }

        public static async Task ClearAllTorrents () {
            await m_ClientEngine!.StopAllAsync ();
            foreach ( var manager in m_TorrentManagers ) {
                if ( manager.Value.Manager == null ) continue;

                await m_ClientEngine.RemoveAsync ( manager.Value.Manager );
            }
            m_TorrentManagers.Clear ();
            m_DownloadedTorrents.Clear ();

            await SaveState ();

            if ( Directory.Exists ( DownloadsPath ) ) Directory.Delete ( DownloadsPath, true );
        }

        public static async Task Finalization ( HttpContext context ) {
            if ( context is null ) throw new ArgumentNullException ( nameof ( context ) );

            await ClearAllTorrents ();

            SendMessageToSocket ( "dt:" );

            context.Response.StatusCode = 200;
            await context.Response.WriteAsync ( "Completed" );
        }

        public static async Task SaveStateAndStop () {
            await SaveState ();

            // close currently actived web sockets
            if ( m_ActiveWebSockets.Any () ) {
                foreach ( var socket in m_ActiveWebSockets.Keys ) {
                    await socket.CloseAsync ( WebSocketCloseStatus.NormalClosure, "server is down", CancellationToken.None );
                }
            }
        }

        public static async Task SaveState () {
            await m_ClientEngine!.SaveStateAsync ( StateFilePath );
            await File.WriteAllTextAsync ( InnerStateFilePath, JsonSerializer.Serialize ( m_TorrentManagers.Values.AsEnumerable (), TorrentStreamSerializerContext.Default.IEnumerableManagerModel ) );
        }

        public static async Task LoadState () {
            if ( !File.Exists ( StateFilePath ) ) return;

            try {
                m_ClientEngine = await ClientEngine.RestoreStateAsync ( StateFilePath );
            } catch {
                m_ClientEngine = new ClientEngine ();
            }
            await m_ClientEngine.StartAllAsync (); // immediate start downloading after starting

            var content = await File.ReadAllTextAsync ( InnerStateFilePath );
            var torrentManagers = JsonSerializer.Deserialize ( content, typeof ( List<ManagerModel> ), TorrentStreamSerializerContext.Default ) as List<ManagerModel>;
            if ( torrentManagers == null ) return;
            foreach ( var torrentManager in torrentManagers ) {
                torrentManager.Manager = m_ClientEngine.Torrents.FirstOrDefault ( a => a.InfoHashes.V1OrV2.ToHex () == torrentManager.MetadataId );
                if ( torrentManager.Manager == null ) continue;
                if ( torrentManager.Manager != null ) torrentManager.Manager.TorrentStateChanged += ManagerTorrentStateChanged;
                m_TorrentManagers.TryAdd ( torrentManager.DownloadPath, torrentManager );
            }
        }

        public static async Task TorrentWebSocket ( HttpContext context ) {
            if ( context.WebSockets.IsWebSocketRequest ) {
                using var webSocket = await context.WebSockets.AcceptWebSocketAsync ();
                m_ActiveWebSockets.TryAdd ( webSocket, true );
                await StartSocketSession ( webSocket );
                if ( !m_ActiveWebSockets.TryRemove ( webSocket, out var result ) ) m_ActiveWebSockets.TryRemove ( webSocket, out var _ );
                return;
            }

            context.Response.StatusCode = StatusCodes.Status400BadRequest;
        }

        private static async Task StartSocketSession ( WebSocket webSocket ) {
            var buffer = new byte[1024].AsMemory ();

            while ( true ) {
                if ( webSocket.State != WebSocketState.Open ) break;

                var receiveResult = await webSocket.ReceiveAsync ( buffer, CancellationToken.None );

                if ( receiveResult.Count == 0 ) break;

                var messageContent = Encoding.UTF8.GetString ( buffer.ToArray () );
                var parts = messageContent.Split ( ":" );
                if ( parts.Length != 2 ) continue;

                switch ( parts[0] ) {
                    case "ds": //download status
                        if ( m_TorrentManagers.Any () ) await webSocket.SendAsync ( GetDownloadStatus (), WebSocketMessageType.Text, true, CancellationToken.None );
                        break;
                }
            }

            if ( m_ActiveWebSockets.ContainsKey ( webSocket ) ) m_ActiveWebSockets.TryRemove ( webSocket, out var _ );
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

        private static ManagerModel? GetTorrentByIdentifier ( string identifier ) {
            var managers = m_TorrentManagers.Values;
            foreach ( var manager in managers ) {
                var files = manager.Manager?.Files
                    .Select ( a => a.Length )
                    .ToArray () ?? Enumerable.Empty<long> ();
                var filesSum = files.Any () ? files.Sum () : 0;
                if ( identifier == DesktopUI.ComputeSha256Hash ( manager.Manager?.Name + ConvertToReadableSize ( filesSum ) ) ) return manager;
            }

            return null;
        }

        public static void StartTorrent ( string identifier ) {
            var torrent = GetTorrentByIdentifier ( identifier );
            if ( torrent == null ) return;

            Task.Run ( torrent.Manager!.StartAsync );
        }

        public static void StopTorrent ( string identifier ) {
            var torrent = GetTorrentByIdentifier ( identifier );
            if ( torrent == null ) return;

            Task.Run ( torrent.Manager!.StopAsync );
        }

        public static void DeleteTorrent ( string identifier ) {
            var torrent = GetTorrentByIdentifier ( identifier );
            if ( torrent == null ) return;

            Task.Run (
                async () => {
                    await RemoveTorrent ( torrent );
                    RemoveTorrentWithoutDownloadPath ( torrent );
                    await SaveState ();
                    SendDeleteAfterSomeTimeout ();
                }
            );
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

        public static async Task<bool> ClearOnlyTorrentByPath ( string downloadPath ) {
            if ( !m_TorrentManagers.ContainsKey ( downloadPath ) ) return false;

            await RemoveTorrentFromTracker ( downloadPath );

            await SaveState ();

            return true;
        }


        public static async Task ClearOnlyTorrent ( HttpContext context ) {
            context.Response.ContentType = "text/plain";
            if ( context.Request.Query.Count != 1 ) {
                context.Response.StatusCode = 204;
                return;
            }

            var downloadPath = GetStringValueFromQuery ( "path", context );

            var operation = await ClearOnlyTorrentByPath ( downloadPath );

            if ( !operation ) {
                await context.Response.WriteAsync ( "Already not exists" );
                return;
            }

            await context.Response.WriteAsync ( "Completed" );

            SendDeleteAfterSomeTimeout ();
        }

        private static void RemoveTorrentWithoutDownloadPath ( ManagerModel torrent ) {
            var downloadPath = "";
            foreach ( var pairs in m_TorrentManagers ) {
                if ( pairs.Value == torrent ) downloadPath = pairs.Key;
            }

            if ( !string.IsNullOrEmpty ( downloadPath ) ) {
                if ( !m_TorrentManagers.TryRemove ( downloadPath, out var _ ) ) {
                    m_TorrentManagers.TryRemove ( downloadPath, out var _ );
                }
                m_DownloadedTorrents.Remove ( downloadPath );
            }
        }

        private static void RemoveTorrentWithDownloadPath ( string downloadPath ) {
            if ( string.IsNullOrEmpty ( downloadPath ) ) return;

            if ( !m_TorrentManagers.TryRemove ( downloadPath, out var _ ) ) {
                m_TorrentManagers.TryRemove ( downloadPath, out var _ );
            }
            m_DownloadedTorrents.Remove ( downloadPath );
        }

        private static async Task RemoveTorrent ( ManagerModel torrent ) {
            if ( torrent.Manager != null ) {
                await torrent.Manager.StopAsync ();
                await m_ClientEngine!.RemoveAsync ( torrent.Manager );
            }
        }

        private static async Task RemoveTorrentFromTracker ( string downloadPath ) {
            if ( m_TorrentManagers.TryGetValue ( downloadPath, out var torrentManager ) ) {
                await RemoveTorrent ( torrentManager );
            }

            RemoveTorrentWithDownloadPath ( downloadPath );
        }

        public static async Task<bool> ClearTorrentAndDataByPath ( string downloadPath ) {
            if ( m_TorrentManagers.TryGetValue ( downloadPath, out var torrent ) ) {
                if ( torrent == null || torrent.Manager == null ) return false;

                try {
                    await RemoveTorrentFromTracker ( downloadPath );
                } catch {
                    //WORKAROUND: try to make once again after some timeout,
                    //sometimes if we try to delete file we get error that file already used in another process
                    //to overcome this error I make this workaround
                    await Task.Delay ( 800 );
                    await RemoveTorrentFromTracker ( downloadPath );
                }
                Directory.Delete ( torrent.Manager.ContainingDirectory, true );
            }

            await SaveState ();

            return true;
        }

        public static async Task ClearTorrentAndData ( HttpContext context ) {
            context.Response.ContentType = "text/plain";
            if ( context.Request.Query.Count != 1 ) {
                context.Response.StatusCode = 204;
                return;
            }

            var downloadPath = GetStringValueFromQuery ( "path", context );

            await ClearTorrentAndDataByPath ( downloadPath );

            await context.Response.WriteAsync ( "Completed" );

            SendDeleteAfterSomeTimeout ();
        }

        private static void SendDeleteAfterSomeTimeout () {
            _ = Task.Run (
                async () => {
                    await Task.Delay ( 800 );
                    SendMessageToSocket ( "dt:" );
                }
            );
        }
    }

}

	 */
}
