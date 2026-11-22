namespace Aniliberty.Unfolded.Routes {

	public static class Maintenance {

		internal static void RegisterRoutes(WebApplication app) {
			app.MapPost("/maintenance/cachebackup", CacheBackup);
			app.MapPost("/maintenance/cacherestore", CacheRestore);
		}

		private static async Task CacheBackup(HttpContext context) {
			throw new NotImplementedException();
		}

		private static async Task CacheRestore(HttpContext context) {
			throw new NotImplementedException();
		}

	}

}
