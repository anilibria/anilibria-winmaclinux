using System.Collections.Concurrent;
using System.Net.WebSockets;
using System.Text;

namespace Aniliberty.Unfolded.Routes
{

	public delegate void WebSocketClientDelegate(string command, string message);

	public class WebSocketHub
	{

		public static readonly ConcurrentDictionary<WebSocket, bool> m_webSockets = new();

		public static readonly Dictionary<string, WebSocketClientDelegate> m_observer = new();

		public static void RegisterRoutes(WebApplication app)
		{
			app.MapGet("/wshub/start", ExternalWebSocket);
		}

		public static async Task ExternalWebSocket(HttpContext context)
		{
			if (!context.WebSockets.IsWebSocketRequest)
			{
				context.Response.StatusCode = StatusCodes.Status400BadRequest;
				return;
			}

			using var webSocket = await context.WebSockets.AcceptWebSocketAsync();
			m_webSockets.TryAdd(webSocket, true);
			await StartSocketSession(webSocket);
		}

		private static ReadOnlyMemory<byte> GetMessageAsBytes(string command, string message) => Encoding.UTF8.GetBytes($"{command}:{message}").AsMemory();

		private static async Task SendToSocket(WebSocket socket, string command, string message)
		{
			await socket.SendAsync(GetMessageAsBytes(command, message), WebSocketMessageType.Text, true, CancellationToken.None);
		}

		public static async Task SendMessage(string command, string message)
		{
			foreach (var webSocket in m_webSockets.Keys)
			{
				await SendToSocket(webSocket, command, message);
			}
		}

		private static async Task StartSocketSession(WebSocket webSocket)
		{
			var buffer = new byte[1024].AsMemory();

			while (true)
			{
				if (webSocket.State != WebSocketState.Open) break;

				var receiveResult = await webSocket.ReceiveAsync(buffer, CancellationToken.None);

				if (receiveResult.Count != 0)
				{
					var messageContent = Encoding.UTF8.GetString(buffer[..receiveResult.Count].ToArray());
					var parts = messageContent.Split(":");
					if (parts.Count() >= 2)
					{
						var command = parts[0];
						var message = parts[1];
						if (m_observer.ContainsKey(command))
						{
							m_observer[command](command, message);
						}
					}
				}
			}

			if (m_webSockets.ContainsKey(webSocket)) m_webSockets.TryRemove(webSocket, out var _);
		}

		public static void RegisterOnCommand(string command, WebSocketClientDelegate handler)
		{
			m_observer.Add(command, handler);
		}

	}

}
