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
			throw new NotImplementedException();
		}

		private static async Task<IResult> Add()
		{
			throw new NotImplementedException();
		}

		internal static List<(SelectionType, int)> GetCounts()
		{
			var models = Releases.GetBySelectionTypes(m_model);

			var result = new List<(SelectionType, int)>();
			foreach (var model in models)
			{
				result.Add((model.Key.Type, model.Value.Count));
			}

			return result;
		}

	}

}
