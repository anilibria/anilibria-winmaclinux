using Aniliberty.Unfolded.Configuration;
using Aniliberty.Unfolded.Models.MySelections;
using Microsoft.AspNetCore.Mvc;
using System.Text.Json;

namespace Aniliberty.Unfolded.Routes
{

	public static class MySelection
	{

		internal static List<SelectionModel> m_model = new List<SelectionModel>();

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapPost("/myselection/add", Add);
			app.MapPut("/myselection/update", Update);
			app.MapDelete("/myselection/remove", Delete);
			app.MapGet("/myselection/counts", Counts);
		}

		private static IResult Counts()
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

		private static async Task<IResult> Add([FromBody] SelectionModel model)
		{
			model.Id = Guid.NewGuid().ToString();
			m_model.Add(model);
			await SaveSelections();

			return Results.Ok();
		}

		private static async Task<IResult> Update([FromBody] SelectionModel model)
		{
			var item = m_model.FirstOrDefault(a => a.Id == model.Id);
			if (item is null) return Results.NotFound();

			var index = m_model.IndexOf(item);
			m_model[index] = model;
			await SaveSelections();

			return Results.Ok();
		}

		private static async Task<IResult> Delete(string id)
		{
			var item = m_model.FirstOrDefault(a => a.Id == id);
			if (item is not null)
			{
				m_model.Remove(item);
			}
			else
			{
				return Results.NotFound();
			}

			await SaveSelections();

			return Results.Ok();
		}

		private static async Task SaveSelections()
		{
			var path = Path.Combine(GlobalConfig.PathToCache(), "selections");
			var json = JsonSerializer.Serialize(m_model, AppJsonSerializerContext.Default.ListSelectionModel);
			await File.WriteAllTextAsync(path, json);
		}

	}

}
