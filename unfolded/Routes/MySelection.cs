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

		internal static Dictionary<SelectionType, int> GetCounts() {
			var result = new Dictionary<SelectionType, int>();


			foreach (var item in m_model)
			{
				//var items = GetByType(item.Type);
			}

			return result;
		}

	}

}
