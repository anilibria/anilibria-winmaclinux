using Aniliberty.Unfolded.Models.Releases;
using Microsoft.AspNetCore.Mvc;

namespace Aniliberty.Unfolded.Routes
{

	public static class CinemaHall
	{

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapPost("/cinemahall/list", FullList);
			app.MapPost("/cinemahall/removeseens", RemoveSeens);
		}

		internal static async Task<IResult> FullList([FromBody] ReleasesListFiltersModel model)
		{
			var releaseIds = AppData.Model.Cinemahall.ToHashSet();

			var filteredItems = Releases.FilterReleases(model)
				.Where(a => releaseIds.Contains(a.Id))
				.ToList();

			return Results.Json(filteredItems, AppJsonSerializerContext.Default);
		}

		internal static async Task<IResult> RemoveSeens()
		{
			var result = new List<int>() ;

			foreach (var id in AppData.Model.Cinemahall.ToHashSet())
			{
				var item = Releases.GetReleaseById(id);
				if (item is null) continue;
				if (item.Status != "Озвучка завершена") continue;

				result.Add(id);
			}

			await AppData.RemoveCinemahall(result);

			return Results.Ok();
		}

	}

}
