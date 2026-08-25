using Aniliberty.Unfolded.Configuration;
using Aniliberty.Unfolded.Routes;
using Microsoft.AspNetCore.StaticFiles;

#if DEBUG
using Microsoft.Extensions.FileProviders;
#endif
#if !DEBUG
using Microsoft.AspNetCore.StaticFiles;
using System.Reflection;
#endif
namespace Aniliberty.Unfolded
{
	public class Program
	{

#if !DEBUG
		private static Dictionary<string, byte[]> GetEmbeddedResources()
		{
			var assembly = Assembly.GetExecutingAssembly();
			var embeddedFolder = "HTMLPages";
			var defaultNamespace = assembly.GetName().Name ?? "";
			var prefix = $"{defaultNamespace}.{embeddedFolder}.";

			var dict = new Dictionary<string, byte[]>(StringComparer.OrdinalIgnoreCase);

			foreach (var resName in assembly.GetManifestResourceNames())
			{
				if (!resName.StartsWith(prefix, StringComparison.OrdinalIgnoreCase)) continue;

				var onlyRelativePath = resName.Substring(prefix.Length);
				var extension = onlyRelativePath[onlyRelativePath.LastIndexOf('.')..];
				var path = onlyRelativePath[..(onlyRelativePath.LastIndexOf('.'))];
				var relativePath = path.Replace('.', '/') + extension;

				using var stream = assembly.GetManifestResourceStream(resName);
				if (stream == null) continue;

				using var ms = new MemoryStream();
				stream.CopyTo(ms);
				dict[relativePath] = ms.ToArray();
			}

			return dict;
		}
#endif

		public static string? GetContentType(string path)
		{
			var provider = new FileExtensionContentTypeProvider();
			if (provider.TryGetContentType(path, out var contentType)) return contentType;

			return null;
		}

		public static async Task Main(string[] args)
		{
			Console.WriteLine("AniLiberty.Unfolded application");
			Console.WriteLine("version 0.0.0\n");

			var builder = WebApplication.CreateSlimBuilder(args);
			builder.WebHost.UseShutdownTimeout(TimeSpan.FromSeconds(2));
			builder.Services.ConfigureHttpJsonOptions(options =>
			{
				options.SerializerOptions.TypeInfoResolverChain.Insert(0, AppJsonSerializerContext.Default);
			});
			// Learn more about configuring OpenAPI at https://aka.ms/aspnet/openapi
			builder.Services.AddOpenApi();
			builder.Services.AddHttpClient();
			builder.Services.AddResponseCompression(options =>
			{
				options.EnableForHttps = true;
			});
			builder.Services.AddCors(options =>
			{
				options.AddPolicy("AllowAll", policy =>
				{
					policy.AllowAnyOrigin().AllowAnyMethod().AllowAnyHeader();
				});
			});

			var app = builder.Build();
			app.UseCors("AllowAll");
			app.UseWebSockets();
			app.MapOpenApi();
			app.UseResponseCompression();
#if DEBUG
			app.UseStaticFiles(
				new StaticFileOptions
				{
					ServeUnknownFileTypes = true,
					FileProvider = new PhysicalFileProvider(Path.Combine(builder.Environment.ContentRootPath, "HTMLPages")),
					RequestPath = new PathString("/static")
				}
			);
#else
			var contents = GetEmbeddedResources();
			app.MapGet("/static/{*path}", async (HttpContext ctx, string? path) =>
			{
				path ??= "";
				path = path.TrimStart('/');

				if (contents.TryGetValue(path, out var bytes))
				{
					return Results.File(bytes, contentType: GetContentType(path) ?? "application/octet-stream");
				}

				return Results.NotFound();
			}).ExcludeFromDescription();
#endif

			app.MapGet("/", () => Results.Redirect("/static/releases.html"));

			GlobalConfig.Initialize(app);
			await AppData.Initialize();
			await Settings.Initialize();
			await Releases.Initialize();
			if (!string.IsNullOrEmpty(Settings.Model.Torrent.PathToDownloads)) await TorrentClient.Initialize();

			Settings.RegisterRoutes(app);
			Synchronize.RegisterRoutes(app);
			Releases.RegisterRoutes(app);
			VideoProxy.RegisterRoutes(app);
			MainMenu.RegisterRoutes(app);
			Authorize.RegisterRoutes(app);
			WebSocketHub.RegisterRoutes(app);
			TorrentClient.RegisterRoutes(app);
			AppData.RegisterRoutes(app);

#if !DEBUG
			// open url in browser
			GlobalConfig.OpenUrl(Releases.IsEmptyData() || AppData.IsFirstStart());
#endif

			app.Run();

			await AppData.Finilize();
			await TorrentClient.Finilize();
			await WebSocketHub.Finilize();
		}
	}

}
