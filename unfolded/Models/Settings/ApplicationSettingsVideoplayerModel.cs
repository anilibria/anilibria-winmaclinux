namespace Aniliberty.Unfolded.Models.Settings
{

	public record ApplicationSettingsVideoplayerModel
	{

		public IEnumerable<string> PrefferedQuality { get; set; } = ["hls1080", "hls720", "hls480"];

		public bool AutoNextOpen { get; set; }

		public bool AutoSkipOpening { get; set; }

		public bool AutoSkipEnding { get; set; }

		public bool ShowReleasePoster { get; set; }

	}

}