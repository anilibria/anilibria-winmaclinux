using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

	public class FranchiseReleasesItemModel
	{

		[JsonPropertyName("sort_order")]
		public int SortOrder { get; set; }

		[JsonPropertyName("release_id")]
		public int ReleaseId { get; set; }

	}

}