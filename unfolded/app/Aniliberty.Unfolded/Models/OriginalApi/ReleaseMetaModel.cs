using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

    internal class ReleaseMetaModel
    {

        [JsonPropertyName("pagination")]
        public ReleasePaginationModel Pagination { get; set; } = new ReleasePaginationModel();

    }

}