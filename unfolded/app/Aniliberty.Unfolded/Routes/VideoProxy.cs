using Aniliberty.Unfolded.Configuration;
using Microsoft.AspNetCore.Mvc;
using System.Net;
using System.Text;

namespace Aniliberty.Unfolded.Routes
{
	public static class VideoProxy
	{

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapGet("/videoproxy/list", ([FromQuery] string path) => PlayList(path));
			app.MapGet("/videoproxy/part", ([FromQuery] string path) => VideoPart(path));
		}

		public static async Task<IResult> PlayList(string path)
		{
			if (string.IsNullOrEmpty(path)) return Results.BadRequest();

			var httpClient = new HttpClient();
			httpClient.DefaultRequestHeaders.Add("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/109.0.0.0 Safari/537.36");
			var uri = new Uri(path);
			var response = await httpClient.GetAsync(uri);
			var medialist = await response.Content.ReadAsStringAsync();

			var partPrefix = $"{uri.Scheme}://";
			var result = new StringBuilder();
			foreach (var line in medialist.Split("\n"))
			{
				if (line.StartsWith(partPrefix))
				{
					result.AppendLine($"http://localhost:{GlobalConfig.ApplicationPort}/videoproxy/part?path=" + line);
				}
				else
				{
					result.AppendLine(line);
				}
			}

			return Results.Content(result.ToString(), "application/x-mpegURL");
		}

		public static async Task<IResult> VideoPart(string path)
		{
			if (string.IsNullOrEmpty(path)) return Results.BadRequest();

			try
			{
				var stream = await TryGetVideoPart(path, useHttp2: true);
				return Results.File(stream, contentType: "video/mp2t", fileDownloadName: "file.ts");
			}
			catch
			{
				Console.WriteLine("Try fallback to HTTP1.1");
				var stream = await TryGetVideoPart(path, useHttp2: false); // falback to HTTP1.1
				return Results.File(stream, contentType: "video/mp2t", fileDownloadName: "file.ts");
			}

			static async Task<MemoryStream> TryGetVideoPart(string path, bool useHttp2)
			{
				var httpClient = new HttpClient();
				if (useHttp2)
				{
					httpClient.DefaultRequestVersion = HttpVersion.Version20;
					httpClient.DefaultVersionPolicy = HttpVersionPolicy.RequestVersionOrLower;
				}
				httpClient.DefaultRequestHeaders.Add("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/109.0.0.0 Safari/537.36 AniLibriaQt/1.0.0");
				var videopart = await httpClient.GetStreamAsync(path);
				var result = new MemoryStream();
				await videopart.CopyToAsync(result);
				return result;
			}
		}

	}

}
