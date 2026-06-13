using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

    internal class ReleaseDataFullCollectionModel
    {

        [JsonPropertyName("data")]
        public IEnumerable<ReleaseDataFullModel> Data { get; set; } = Enumerable.Empty<ReleaseDataFullModel>();

    }

}
