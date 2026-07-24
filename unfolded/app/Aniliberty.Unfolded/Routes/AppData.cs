using Aniliberty.Unfolded.Configuration;
using Aniliberty.Unfolded.Models.AppDatas;
using Microsoft.AspNetCore.Mvc;
using System.Text.Json;

namespace Aniliberty.Unfolded.Routes
{

	public static class AppData
	{

		private static AppDataModel m_appData = new AppDataModel();

		internal static AppDataModel Model => m_appData;

		public static async Task Initialize()
		{
			var path = Path.Combine(GlobalConfig.PathToCache(), "appdata");
			if (File.Exists(path))
			{
				var json = await File.ReadAllTextAsync(path);
				var serialized = JsonSerializer.Deserialize(json, AppJsonSerializerContext.Default.AppDataModel);
				if (serialized != null) m_appData = serialized;
			}
		}

		public static async Task Finilize()
		{
			m_appData.LastAppStart = DateTime.Now;
			m_appData.AppStartCounter += 1;
			await SaveSettings();
		}

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapGet("/appdata/hidedreleases", () => Results.Json(m_appData.HidedReleases, AppJsonSerializerContext.Default));
			app.MapGet("/appdata/cinemahall", () => Results.Json(m_appData.Cinemahall, AppJsonSerializerContext.Default));
			app.MapPost("/appdata/hidedreleases/{action}", ([FromRoute] string action, [FromBody] IEnumerable<int> ids) => EditHidedReleases(action, ids));
			app.MapPost("/appdata/cinemahall/{action}", ([FromRoute] string action, [FromBody] IEnumerable<int> ids) => EditCinemahall(action, ids));
		}

		public static async Task<IResult> EditHidedReleases(string action, IEnumerable<int> ids)
		{
			if (ids is null || !ids.Any()) return Results.NoContent();

			var lowerAction = action.ToLowerInvariant();
			if (lowerAction == "add")
			{
				var newIds = ids.Except(m_appData.HidedReleases);
				m_appData.HidedReleases.AddRange(newIds);
			}
			if (lowerAction == "remove") m_appData.HidedReleases = m_appData.HidedReleases.Where(a => !ids.Contains(a)).ToList();

			await SaveSettings();

			return Results.StatusCode(200);
		}

		public static async Task<IResult> EditCinemahall(string action, IEnumerable<int> ids)
		{
			if (ids is null || !ids.Any()) return Results.NoContent();

			var lowerAction = action.ToLowerInvariant();
			if (lowerAction == "add")
			{
				var newIds = ids.Except(m_appData.Cinemahall);
				m_appData.Cinemahall.AddRange(newIds);
			}
			if (lowerAction == "remove") m_appData.Cinemahall = m_appData.Cinemahall.Where(a => !ids.Contains(a)).ToList();

			await SaveSettings();

			return Results.StatusCode(200);
		}

		private static async Task SaveSettings()
		{
			var path = Path.Combine(GlobalConfig.PathToCache(), "appdata");
			var json = JsonSerializer.Serialize(m_appData, AppJsonSerializerContext.Default.AppDataModel);
			await File.WriteAllTextAsync(path, json);
		}

	}

}
