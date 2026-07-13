using Aniliberty.Unfolded.Configuration;
using Aniliberty.Unfolded.Models.Settings;
using Microsoft.AspNetCore.Mvc;
using System.Text.Json;

namespace Aniliberty.Unfolded.Routes
{

	public static class Settings
	{

		private static ApplicationSettingsModel m_settings = new ApplicationSettingsModel();

		internal static ApplicationSettingsModel Model => m_settings;

		public static async Task Initialize()
		{
			var path = Path.Combine(GlobalConfig.PathToCache(), "settings");
			if (File.Exists(path))
			{
				var json = await File.ReadAllTextAsync(path);
				var serialized = JsonSerializer.Deserialize(json, AppJsonSerializerContext.Default.ApplicationSettingsModel);
				if (serialized != null) m_settings = serialized;
			}
		}

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapGet("/settings/bypage", ([FromQuery] string page) => ByPage(page));
			app.MapPost("/settings/save/releases", ([FromBody] ApplicationSettingsReleasesModel model, CancellationToken cancellationToken) => SaveReleases(model, cancellationToken));
			app.MapPost("/settings/save/videoplayer", ([FromBody] ApplicationSettingsVideoplayerModel model, CancellationToken cancellationToken) => SaveVideoPlayer(model, cancellationToken));
		}

		private static IResult ByPage(string page)
		{

			switch (page)
			{
				case "releases": return Results.Json(m_settings.Releases, AppJsonSerializerContext.Default);
				case "videoplayer": return Results.Json(m_settings.VideoPlayer, AppJsonSerializerContext.Default);
				default: return Results.NotFound();
			}
		}

		private static async Task<IResult> SaveVideoPlayer(ApplicationSettingsVideoplayerModel model, CancellationToken cancellationToken)
		{
			if (model == null) return Results.StatusCode(400);

			m_settings.VideoPlayer = model;

			var json = JsonSerializer.Serialize(m_settings, AppJsonSerializerContext.Default.ApplicationSettingsModel);
			var path = Path.Combine(GlobalConfig.PathToCache(), "settings");
			await File.WriteAllTextAsync(path, json, cancellationToken);

			return Results.Ok();
		}

		private static async Task<IResult> SaveReleases(ApplicationSettingsReleasesModel model, CancellationToken cancellationToken)
		{
			if (model == null) return Results.StatusCode(400);

			m_settings.Releases = model;

			var json = JsonSerializer.Serialize(m_settings, AppJsonSerializerContext.Default.ApplicationSettingsModel);
			var path = Path.Combine(GlobalConfig.PathToCache(), "settings");
			await File.WriteAllTextAsync(path, json, cancellationToken);

			return Results.Ok();
		}

	}

}
