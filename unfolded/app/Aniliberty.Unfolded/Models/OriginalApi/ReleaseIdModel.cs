using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{
    internal class ReleaseIdModel
    {

		[JsonPropertyName("meta")]
		public ReleaseMetaModel Meta { get; set; } = new ReleaseMetaModel();

        [JsonPropertyName("data")]
        public IEnumerable<ReleaseDataIdModel> Data { get; set; } = Enumerable.Empty<ReleaseDataIdModel>();

    }
}
