namespace Aniliberty.Unfolded.Models.Releases
{
	public class ReleaseDisplayEpisodeModel
	{

		public string Id { get; set; } = "";

		public string Name { get; set; } = "";

		public string NameEnglish { get; set; } = "";

		public decimal Ordinal { get; set; }

		public int SortOrder { get; set; }

		public string Hls480 { get; set; } = "";

		public string Hls720 { get; set; } = "";

		public string Hls1080 { get; set; } = "";

		public string RutubeId { get; set; } = "";

		public string YoutubeId { get; set; } = "";

		public int? OpeningStart { get; set; }

		public int? OpeningEnd { get; set; }

		public int? EndingStart { get; set; }

		public int? EndingEnd { get; set; }

		public string Preview { get; set; } = "";

	}

}
