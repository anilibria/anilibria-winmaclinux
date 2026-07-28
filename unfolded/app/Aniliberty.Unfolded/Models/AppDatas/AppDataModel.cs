namespace Aniliberty.Unfolded.Models.AppDatas
{

	internal class AppDataModel
	{

		public List<int> HidedReleases { get; set; } = Enumerable.Empty<int>().ToList();

		public List<int> Cinemahall { get; set; } = Enumerable.Empty<int>().ToList();

		public Dictionary<int, DateTime> HistoryWatch { get; set; } = new Dictionary<int, DateTime>();

		public Dictionary<int, (DateTime hit, string videoId, long time)> HistoryWatchVideo { get; set; } = new Dictionary<int, (DateTime hit, string videoId, long time)>();

		public DateTime LastAppStart { get; set; } = DateTime.Now;

		public int AppStartCounter { get; set; } = 0;

		//TODO: local groups

	}

}
