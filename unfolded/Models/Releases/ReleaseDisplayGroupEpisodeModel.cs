namespace Aniliberty.Unfolded.Models.Releases
{
	public class ReleaseDisplayGroupEpisodeModel
	{

		public int ReleaseId { get; set; }

		public string Name { get; set; } = "";

		public string Poster { get; set; } = "";

		public string Mode { get; set; } = "";

		public IEnumerable<ReleaseDisplayEpisodeModel> Episodes { get; set; } = Enumerable.Empty<ReleaseDisplayEpisodeModel>();

	}

}
