using Aniliberty.Unfolded.Models.Releases;
using Microsoft.AspNetCore.Mvc;

namespace Aniliberty.Unfolded.Routes {

	internal static class Series {

		public static void RegisterRoutes(WebApplication app) {
			app.MapPost("/series/list", FullList);
		}

		internal static async Task<IResult> FullList([FromBody] ReleasesListFiltersModel model) {
			var releaseIds = Releases.GetFranchises()
				.SelectMany(a => a.Releases)
				.Select(a => a.Id)
				.ToArray();

			var filteredItems = Releases.FilterReleases(model)
				.Where(a => releaseIds.Contains(a.Id))
				.ToList();

			return Results.Json(filteredItems, AppJsonSerializerContext.Default);
		}


	}

}
