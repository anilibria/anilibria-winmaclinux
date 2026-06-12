using Aniliberty.Unfolded.Models.OriginalApi;
using System.Text.Json;

namespace Aniliberty.Unfolded.Helpers
{

	internal static class OriginalApiMaker
	{

		public static string ApiDomain = "https://api.anilibria.app";

		static public async Task<ReleaseIdModel> GetPageIds(int page, HttpClient httpClient, int countOnPages = 50)
		{
			var dictionary = new Dictionary<string, string>();
			dictionary["page"] = page.ToString();
			dictionary["limit"] = countOnPages.ToString();
			dictionary["f[sorting]"] = "FRESH_AT_DESC";
			dictionary["include"] = "id";

			var serializeOptions = new JsonSerializerOptions
			{
				PropertyNamingPolicy = JsonNamingPolicy.SnakeCaseLower
			};
			HttpResponseMessage pageContent;
			try
			{
				pageContent = await httpClient.GetAsync($"{ApiDomain}/api/v1/anime/catalog/releases?" + string.Join("&", dictionary.Select(a => $"{a.Key}={a.Value}")));
			}
			catch (Exception ex)
			{
				throw new Exception($"Can't make HTTP request for page {page}: {ex.Message}", ex);
			}
			if (pageContent == null) throw new Exception($"Can't read content for page {page}");

			string jsonContent = "";
			try
			{
				jsonContent = await pageContent.Content.ReadAsStringAsync();
			}
			catch (Exception ex)
			{
				throw new Exception($"Can't read content for page {page}", ex);
			}
			if (jsonContent == null) throw new Exception($"Can't read content for page {page}");

			try
			{
				var content = (ReleaseIdModel?)JsonSerializer.Deserialize(jsonContent, typeof(ReleaseIdModel), AppJsonSerializerContext.Default);
				return content == null ? throw new Exception($"Can't serialize response for page {page}") : content;
			}
			catch (Exception ex)
			{
				throw new Exception($"Can't deserialize content for page {page} - {jsonContent}", ex);
			}
		}

		static public async Task<IEnumerable<ScheduleReleaseModel>> GetFullSchedule(HttpClient httpClient)
		{
			return await PerformRequest<IEnumerable<ScheduleReleaseModel>>(httpClient, $"{ApiDomain}/api/v1/anime/schedule/week", "schedule");
		}

		static public async Task<IEnumerable<StringValueItem>> GetAgeRatings(HttpClient httpClient)
		{
			return await PerformRequest<IEnumerable<StringValueItem>>(httpClient, $"{ApiDomain}/api/v1/anime/catalog/references/age-ratings", "age ratings");
		}

		static public async Task<IEnumerable<IntegerValueItem>> GetGenres(HttpClient httpClient)
		{
			return await PerformRequest<IEnumerable<IntegerValueItem>>(httpClient, $"{ApiDomain}/api/v1/anime/catalog/references/genres", "genres");
		}

		static public async Task<IEnumerable<StringValueItem>> GetSeasons(HttpClient httpClient)
		{
			return await PerformRequest<IEnumerable<StringValueItem>>(httpClient, $"{ApiDomain}/api/v1/anime/catalog/references/seasons", "seasons");
		}

		static public async Task<IEnumerable<StringValueItem>> GetTypes(HttpClient httpClient)
		{
			return await PerformRequest<IEnumerable<StringValueItem>>(httpClient, $"{ApiDomain}/api/v1/anime/catalog/references/types", "types");
		}

		static public async Task<IEnumerable<FranchiseModel>> GetAllFranchises(HttpClient httpClient)
		{
			return await PerformRequest<IEnumerable<FranchiseModel>>(httpClient, $"{ApiDomain}/api/v1/anime/franchises", "franchise releases");
		}

		static public async Task<FranchiseReleasesModel> GetFranchisesReleases(HttpClient httpClient, string id)
		{
			return await PerformRequest<FranchiseReleasesModel>(httpClient, $"{ApiDomain}/api/v1/anime/franchises/" + id, "franchise releases");
		}

		static public async Task<ReleaseDataFullCollectionModel> GetReleasesInnerCollections(HttpClient httpClient, IEnumerable<int> releasesId)
		{
			return await PerformRequest<ReleaseDataFullCollectionModel>(httpClient, $"{ApiDomain}/api/v1/anime/releases/list/?ids={string.Join(',', releasesId)}&page=1&limit=50", "releases with full data");
		}

		private static async Task<T> PerformRequest<T>(HttpClient httpClient, string url, string requestName)
		{
			var serializeOptions = new JsonSerializerOptions
			{
				PropertyNamingPolicy = JsonNamingPolicy.SnakeCaseLower
			};
			HttpResponseMessage pageContent;
			try
			{
				pageContent = await httpClient.GetAsync(url);
			}
			catch (Exception ex)
			{
				throw new Exception($"Can't make HTTP request for {requestName} ({url})", ex);
			}
			if (pageContent == null) throw new Exception($"Can't read content for {requestName}");

			string jsonContent;
			try
			{
				jsonContent = await pageContent.Content.ReadAsStringAsync();
			}
			catch (Exception ex)
			{
				throw new Exception($"Can't read content for {requestName}", ex);
			}
			if (string.IsNullOrEmpty(jsonContent)) throw new Exception($"JSON content for {requestName} is empty!");

			var content = (T?)JsonSerializer.Deserialize(jsonContent, typeof(T), AppJsonSerializerContext.Default);
			return content == null ? throw new Exception($"Can't serialize response for {requestName}") : content;
		}

		public static async Task DownloadPoster(HttpClient httpClient, string url, string path)
		{
			HttpResponseMessage pageContent;
			try
			{
				pageContent = await httpClient.GetAsync(url);
			}
			catch (Exception ex)
			{
				throw new Exception($"Can't make HTTP request for {url}", ex);
			}
			var stream = await pageContent.Content.ReadAsStreamAsync();
			using var file = File.Create(path);
			await stream.CopyToAsync(file);
		}

	}


}
