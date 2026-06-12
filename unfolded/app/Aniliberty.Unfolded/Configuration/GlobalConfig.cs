using System.Diagnostics;
using System.Net.NetworkInformation;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Aniliberty.Unfolded.Configuration
{

	public static class GlobalConfig
	{

		private static int m_applicationPort = 8688;

		private static string m_pathToCache = "";

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static int ApplicationPort() => m_applicationPort;

		[MethodImpl(MethodImplOptions.AggressiveInlining)]
		public static string PathToCache() => m_pathToCache;

		public static void Initialize(WebApplication app)
		{
			var availablePort = GetAvailablePort(8688);
			var localDataFolder = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
			m_pathToCache = Path.Combine(localDataFolder, "AnilibertyUnfolded");
			if (!Directory.Exists(m_pathToCache)) Directory.CreateDirectory(m_pathToCache);

			m_applicationPort = availablePort;

			app.Urls.Add("http://localhost:" + m_applicationPort);
		}

		public static void OpenUrl()
		{
			var url = "http://localhost:" + m_applicationPort;

			if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
			{
				Process.Start(new ProcessStartInfo { FileName = url, UseShellExecute = true });
			}
			else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
			{
				Process.Start("xdg-open", url);
			}
			else if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
			{
				Process.Start("open", url);
			}
		}

		private static int GetAvailablePort(int startingPort)
		{
			var properties = IPGlobalProperties.GetIPGlobalProperties();

			var closedPorts = properties.GetActiveTcpConnections()
				.Where(n => n.LocalEndPoint.Port >= startingPort)
				.Select(n => n.LocalEndPoint.Port)
				.Union(
					properties.GetActiveTcpListeners()
						.Where(n => n.Port >= startingPort)
						.Select(n => n.Port)
				)
				.Union(
					properties.GetActiveUdpListeners()
						.Where(n => n.Port >= startingPort)
						.Select(n => n.Port)
				);

			return Enumerable.Range(startingPort, ushort.MaxValue)
				.FirstOrDefault(i => !closedPorts.Contains(i));
		}

	}

}
