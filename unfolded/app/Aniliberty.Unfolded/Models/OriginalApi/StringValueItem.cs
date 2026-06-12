using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded.Models.OriginalApi
{

    internal record StringValueItem
    {


        public string Value { get; set; } = "";

        [JsonIgnore ( Condition = JsonIgnoreCondition.WhenWritingDefault )]
        public string Description { get; set; } = "";

        [JsonIgnore ( Condition = JsonIgnoreCondition.WhenWritingDefault )]
        public string Label { get; set; } = "";

    }

}
