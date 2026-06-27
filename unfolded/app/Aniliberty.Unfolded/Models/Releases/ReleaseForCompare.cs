namespace Aniliberty.Unfolded.Models.Releases
{

	/// <summary>
	/// Relase for compare in notifications.
	/// </summary>
	public record ReleaseForCompare
	{

		public int Id { get; init; }

		public int CountVideos { get; set; }

		public long TorrentsSize { get; set; }

	}


}
