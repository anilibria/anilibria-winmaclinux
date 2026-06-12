using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

    public record TimeRange
    {

        [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
        public int? Start { get; init; }

        [JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingNull)]
        public int? Stop { get; init; }

    }

}