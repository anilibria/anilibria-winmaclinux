using System.Text.Json;

namespace Aniliberty.Unfolded.Helpers
{

	/// <summary>
	/// Helper for JSON.
	/// </summary>
	internal static class JsonHelpers
	{

		public static string SerializeToJson<T>(T model) => JsonSerializer.Serialize(
			model,
			typeof(T),
			AppJsonSerializerContext.Default
		);

		public static T? DeserializeFromJson<T>(string json) => (T?)JsonSerializer.Deserialize(
			json,
			typeof(T),
			AppJsonSerializerContext.Default
		);

	}

}
