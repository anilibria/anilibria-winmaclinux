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
			app.MapPost("/appdata/watchrelease", ([FromQuery] int id) => WatchRelease(id));
			app.MapPost("/appdata/watchvideorelease", ([FromQuery] int id, [FromQuery] string episode, [FromQuery] long time) => WatchVideoRelease(id, episode, time));
			app.MapGet("/appdata/watchvideorelease", ([FromQuery] int id) => GetWatchVideoRelease(id));
		}

		private static IResult WatchRelease(int id)
		{
			if (m_appData.HistoryWatch.TryGetValue(id, out var _))
			{
				m_appData.HistoryWatch[id] = DateTime.Now;
			}
			else
			{
				m_appData.HistoryWatch.Add(id, DateTime.Now);
			}

			return Results.Ok();
		}

		private static IResult WatchVideoRelease(int id, string episode, long time)
		{
			if (m_appData.HistoryWatchVideo.TryGetValue(id, out var _))
			{
				m_appData.HistoryWatchVideo[id] = new AppDataModelWatchVideoModel { Hit = DateTime.Now, VideoId = episode, Time = time };
			}
			else
			{
				m_appData.HistoryWatchVideo.Add(id, new AppDataModelWatchVideoModel { Hit = DateTime.Now, VideoId = episode, Time = time });
			}

			return Results.Ok();
		}

		private static IResult GetWatchVideoRelease(int id)
		{

			if (m_appData.HistoryWatchVideo.TryGetValue(id, out var item))
			{
				return Results.Json(item, AppJsonSerializerContext.Default);
			}

			return Results.Content("null", "application/json");
		}

		public static async Task<IResult> EditHidedReleases(string action, IEnumerable<int> ids)
		{
			if (ids is null || !ids.Any()) return Results.NoContent();

			var lowerAction = action.ToLowerInvariant();
			if (lowerAction == "add")
			{
				var newIds = ids.Except(m_appData.HidedReleases);
				foreach (var newId in newIds) m_appData.HidedReleases.Add(newId);
			}
			if (lowerAction == "remove")
			{
				foreach (var removeId in ids) m_appData.HidedReleases.Remove(removeId);
			}

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

		public static bool InHidedReleases(int id) => m_appData.HidedReleases.Contains(id);

		public static bool InCinemahall(int id) => m_appData.Cinemahall.Contains(id);

		public static bool IsNeedToUpdateCommonThings() => m_appData.AppStartCounter % 3 == 0;

	}

}
