namespace Aniliberty.Unfolded.Models.Settings
{

	public class ApplicationSettingsReleasesModel
	{

		/// <summary>
		/// Started section on page.
		/// </summary>
		public int StartedSection { get; set; } = 0;

		/// <summary>
		/// Started subsection on page.
		/// </summary>
		public int StartedSubSection { get; set; } = -1;

		/// <summary>
		/// Notification mode (disabled, enabled full or only favorites)
		/// </summary>
		public int NotificationMode { get; set; } = 0;

	}

}