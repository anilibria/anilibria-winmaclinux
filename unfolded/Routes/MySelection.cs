using Aniliberty.Unfolded.Configuration;
using Aniliberty.Unfolded.Helpers;
using Aniliberty.Unfolded.Models.MySelections;
using Microsoft.AspNetCore.Mvc;

namespace Aniliberty.Unfolded.Routes
{

	public static class MySelection
	{

		internal static List<SelectionModel> m_model = new List<SelectionModel>();

		internal static async Task Initialize()
		{
			var selectionsPath = Path.Combine(GlobalConfig.PathToCache(), "selections");
			if (File.Exists(selectionsPath))
			{
				var json = await File.ReadAllTextAsync(selectionsPath);
				m_model = (JsonHelpers.DeserializeFromJson<List<SelectionModel>>(json) ?? new List<SelectionModel>())
					.OrderBy(a => a.Order)
					.ToList();
			}
		}

		internal static void RegisterRoutes(WebApplication app)
		{
			app.MapPost("/myselection/add", Add);
			app.MapPut("/myselection/update", Update);
			app.MapDelete("/myselection/remove", Delete);
			app.MapGet("/myselection/counts", Counts);
			app.MapGet("/myselection/allselections", AllSelections);
			app.MapPut("/myselection/changeexpand", ChangeExpand);
			app.MapPut("/myselection/changeorder", ChangeSort);
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
						Title = model.Key.Name
					}
				);
			}

			return Results.Json(result, AppJsonSerializerContext.Default);
		}

		private static IResult AllSelections()
		{
			var models = Releases.GetBySelectionTypes(m_model);

			var result = new List<SelectionCountModel>();
			foreach (var model in models)
			{
				result.Add(
					new SelectionCountModel
					{
						Id = model.Key.Id,
						Count = model.Value.Count,
						Type = model.Key.Type,
						Title = model.Key.Name,
						Expanded = model.Key.Expanded,
						Releases = model.Value
							.Select(
								a => new SelectionCountReleaseModel
								{
									Description = a.Description,
									Id = a.Id,
									Poster = a.Poster,
									Title = a.Title
								}
							)
					}
				);
			}

			return Results.Json(result, AppJsonSerializerContext.Default);
		}

		private static async Task<IResult> Add([FromBody] SelectionModel model)
		{
			model.Id = Guid.NewGuid().ToString();
			model.Order = m_model.Any() ? m_model.Max(a => a.Order) + 1 : 1;
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

		private static async Task<IResult> ChangeExpand([FromQuery] string id, [FromQuery] bool expand)
		{
			var item = m_model.FirstOrDefault(a => a.Id == id);
			if (item is not null)
			{
				item.Expanded = expand;
				await SaveSelections();
				return Results.Ok();
			}

			return Results.NoContent();
		}

		private static async Task<IResult> ChangeSort([FromQuery] string movedId, [FromQuery] string dropId)
		{
			var moved = m_model.FirstOrDefault(a => a.Id == movedId);
			var drop = m_model.FirstOrDefault(a => a.Id == dropId);
			if (moved is null || drop is null) return Results.BadRequest();

			var dropIndex = m_model.IndexOf(drop);
			var isLess = m_model.IndexOf(moved) < dropIndex;
			m_model.Remove(moved);
			m_model.Insert(dropIndex + (isLess ? 1 : 0), moved);

			var iterator = 1;
			foreach (var item in m_model)
			{
				item.Order = iterator;
				iterator++;
			}

			await SaveSelections();

			return Results.Ok();
		}

		private static async Task SaveSelections()
		{
			var path = Path.Combine(GlobalConfig.PathToCache(), "selections");
			var json = JsonHelpers.SerializeToJson(m_model);
			await File.WriteAllTextAsync(path, json);
		}

	}

}
