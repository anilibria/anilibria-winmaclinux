using Aniliberty.Unfolded.Configuration;
using Aniliberty.Unfolded.Routes;
using Microsoft.Extensions.FileProviders;

namespace Aniliberty.Unfolded
{
	public class Program
	{
		public static async Task Main(string[] args)
		{
			var builder = WebApplication.CreateSlimBuilder(args);

			builder.Services.ConfigureHttpJsonOptions(options =>
			{
				options.SerializerOptions.TypeInfoResolverChain.Insert(0, AppJsonSerializerContext.Default);
			});

			// Learn more about configuring OpenAPI at https://aka.ms/aspnet/openapi
			builder.Services.AddOpenApi();
			builder.Services.AddHttpClient();

			var app = builder.Build();

			app.MapOpenApi();

			app.UseStaticFiles(
				new StaticFileOptions
				{
					ServeUnknownFileTypes = true,
					FileProvider = new PhysicalFileProvider(Path.Combine(builder.Environment.ContentRootPath, "HTMLPages")),
					RequestPath = new PathString("/static")
				}
			);

			/*Todo[] sampleTodos =
			[
				new(1, "Walk the dog"),
				new(2, "Do the dishes", DateOnly.FromDateTime(DateTime.Now)),
				new(3, "Do the laundry", DateOnly.FromDateTime(DateTime.Now.AddDays(1))),
				new(4, "Clean the bathroom"),
				new(5, "Clean the car", DateOnly.FromDateTime(DateTime.Now.AddDays(2)))
			];

			var todosApi = app.MapGroup("/todos");
			todosApi.MapGet("/", () => sampleTodos)
					.WithName("GetTodos");

			todosApi.MapGet("/{id}", Results<Ok<Todo>, NotFound> (int id) =>
				sampleTodos.FirstOrDefault(a => a.Id == id) is { } todo
					? TypedResults.Ok(todo)
					: TypedResults.NotFound())
				.WithName("GetTodoById");*/
			//app.MapGet("/releases/release", ([FromQuery] int id) => Release(id));
			app.MapGet("/", () => Results.Content("Lalala"));

			GlobalConfig.Initialize(app);
			await Releases.Initialize();

			Synchronize.RegisterRoutes(app);
			Releases.RegisterRoutes(app);
			VideoProxy.RegisterRoutes(app);
			MainMenu.RegisterRoutes(app);

			//GlobalConfig.OpenUrl();

			app.Run();
		}
	}

}
