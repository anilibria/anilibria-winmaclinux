namespace Aniliberty.Unfolded.Routes {

	public static class Maintenance {

		internal static void RegisterRoutes(WebApplication app) {
			app.MapGet("/maintenance/posteraddress", PosterAddress);
			app.MapPost("/maintenance/cachebackup", CacheBackup);
			app.MapPost("/maintenance/cacherestore", CacheRestore);
		}

		private static async Task<IResult> PosterAddress() {
			return Results.Json(
				Settings.Model.Maintenance.PosterAddress switch {
					0 => "https://aniliberty.top",
					1 => "https://anilibria.top",
					2 => "https://api.anilibria.app",
					100 => Settings.Model.Maintenance.PosterAddressCustom,
					_ => ""
				},
				AppJsonSerializerContext.Default
			);
		}

		private static async Task CacheBackup(HttpContext context) {
			throw new NotImplementedException();
		}

		private static async Task CacheRestore(HttpContext context) {
			throw new NotImplementedException();
		}

	}

}
