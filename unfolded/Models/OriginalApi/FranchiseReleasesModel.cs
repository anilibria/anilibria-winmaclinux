using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

    internal class FranchiseReleasesModel
    {

        [JsonPropertyName("franchise_releases")]
        public IEnumerable<FranchiseReleasesItemModel> FranchiseReleases { get; set; } = Enumerable.Empty<FranchiseReleasesItemModel>();

    }

}
