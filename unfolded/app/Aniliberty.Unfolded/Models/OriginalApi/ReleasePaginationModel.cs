using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

	internal class ReleasePaginationModel
	{

		[JsonPropertyName("total")]
		public int Total { get; set; }

		[JsonPropertyName("count")]
		public int Count { get; set; }

		[JsonPropertyName("per_page")]
		public int PerPage { get; set; }

		[JsonPropertyName("current_page")]
		public int CurrentPage { get; set; }

		[JsonPropertyName("total_pages")]
		public int TotalPages { get; set; }

	}

}