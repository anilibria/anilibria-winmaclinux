namespace Aniliberty.Unfolded.Models.Settings
{

	public record ApplicationSettingsVideoplayerModel
	{

		public IEnumerable<string> PrefferedQuality { get; set; } = ["hls1080", "hls720", "hls480"];

	}

}