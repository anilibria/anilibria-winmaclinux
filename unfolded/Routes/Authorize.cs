using Aniliberty.Unfolded.Helpers;
using Microsoft.AspNetCore.Mvc;

namespace Aniliberty.Unfolded.Routes
{

	public static class Authorize
	{

		public const string CookieName = "uft";

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapPost(
				"/auth/login",
				([FromQuery] string username, [FromBody] string password, HttpContext context, IHttpClientFactory clientFactory)
					=> Login(clientFactory, context, username, password)
			);
			app.MapGet("/auth/logout", (IHttpClientFactory clientFactory, HttpContext context) => Logout(clientFactory, context));
		}

		public static async Task<IResult> Login(IHttpClientFactory clientFactory, HttpContext context, string userName, string password)
		{
			try
			{
				var response = await OriginalApiMaker.AuthorizeByLoginPass(clientFactory.CreateClient(), userName, password);

				var cookieOptions = new CookieOptions
				{
					Path = "/",
					Domain = "localhost",
					Secure = true,
					HttpOnly = true,
					SameSite = SameSiteMode.Strict,
					Expires = DateTimeOffset.UtcNow.AddDays(100),
				};

				context.Response.Cookies.Append(CookieName, response.Token, cookieOptions);

				return Results.Ok("success");
			}
			catch
			{
				return Results.Ok("failed");
			}
		}

		public static IResult Logout(IHttpClientFactory clientFactory, HttpContext context)
		{
			var options = new CookieOptions
			{
				Path = "/",
				Domain = "localhost",
				Secure = true,
				HttpOnly = true,
				SameSite = SameSiteMode.Strict,
			};

			context.Response.Cookies.Delete(CookieName, options);
			return Results.Ok("success");
		}

	}

}
