namespace Aniliberty.Unfolded.Models.AppDatas
{

	internal class AppDataModel
	{

		public HashSet<int> HidedReleases { get; set; } = Enumerable.Empty<int>().ToHashSet();

		public List<int> Cinemahall { get; set; } = Enumerable.Empty<int>().ToList();

		public Dictionary<int, DateTime> HistoryWatch { get; set; } = new Dictionary<int, DateTime>();

		public Dictionary<int, AppDataModelWatchVideoModel> HistoryWatchVideo { get; set; } = new Dictionary<int, AppDataModelWatchVideoModel>();

		public DateTime LastAppStart { get; set; } = DateTime.Now;

		public int AppStartCounter { get; set; } = 0;

		//TODO: local groups

	}

	internal class AppDataModelWatchVideoModel
	{
		public DateTime Hit { get; set; }

		public string VideoId { get; set; } = "";

		public long Time { get; set; }

	}

}
