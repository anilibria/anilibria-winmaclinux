namespace Aniliberty.Unfolded.Routes
{

	public static class MySelection
	{

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapPost("/myselection/add", Add);
			app.MapGet("/myselection/counts", Counts);
		}

		private static async Task<IResult> Counts()
		{
			throw new NotImplementedException();
		}

		private static async Task<IResult> Add()
		{
			throw new NotImplementedException();
		}

	}

}
