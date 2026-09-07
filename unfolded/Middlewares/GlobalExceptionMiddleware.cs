using Microsoft.AspNetCore.Diagnostics;

namespace Aniliberty.Unfolded.Middlewares
{
	public class GlobalExceptionMiddleware : IExceptionHandler
	{
		public async ValueTask<bool> TryHandleAsync(
			HttpContext httpContext,
			Exception exception,
			CancellationToken cancellationToken)
		{
			Console.WriteLine($"Crash happened: {exception.Message}. {exception.StackTrace}");

			httpContext.Response.StatusCode = StatusCodes.Status500InternalServerError;

			await httpContext.Response.WriteAsync("Error happened", cancellationToken);

			return true;
		}
	}
}
