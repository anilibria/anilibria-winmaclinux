namespace Aniliberty.Unfolded.Models.Settings
{

	internal class ApplicationSettingsModel
	{

		public ApplicationSettingsReleasesModel Releases { get; set; } = new ApplicationSettingsReleasesModel();

		public ApplicationSettingsVideoplayerModel VideoPlayer { get; set; } = new ApplicationSettingsVideoplayerModel();

		public ApplicationSettingsTorrentModel Torrent { get; set; } = new ApplicationSettingsTorrentModel();

	}

}
