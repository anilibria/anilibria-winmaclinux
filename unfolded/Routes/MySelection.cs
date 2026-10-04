using Aniliberty.Unfolded.Models.MySelections;

namespace Aniliberty.Unfolded.Routes
{

	public static class MySelection
	{

		internal static IEnumerable<SelectionModel> m_model = Enumerable.Empty<SelectionModel>();

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapPost("/myselection/add", Add);
			app.MapGet("/myselection/counts", Counts);
		}

		private static async Task<IResult> Counts()
		{
			var models = Releases.GetBySelectionTypes(m_model);

			var result = new List<CountModel>();
			foreach (var model in models)
			{
				result.Add(
					new CountModel
					{
						Count = model.Value.Count,
						Type = model.Key.Type,
						Name = model.Key.Name
					}
				);
			}

			return Results.Json(result, AppJsonSerializerContext.Default);
		}

		private static async Task<IResult> Add()
		{
			throw new NotImplementedException();
		}

	}

}
