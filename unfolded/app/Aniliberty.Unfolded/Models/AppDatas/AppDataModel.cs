namespace Aniliberty.Unfolded.Models.AppDatas
{

	internal record WatchItem(int Id, DateTime Hit);

	internal class AppDataModel
	{

		public List<int> HidedReleases { get; set; } = Enumerable.Empty<int>().ToList();

		public List<int> Cinemahall { get; set; } = Enumerable.Empty<int>().ToList();

		public List<WatchItem> HistoryWatch { get; set; } = Enumerable.Empty<WatchItem>().ToList();

		public List<WatchItem> HistoryWatchVideo { get; set; } = Enumerable.Empty<WatchItem>().ToList();

		public DateTime LastAppStart { get; set; } = DateTime.Now;

		public int AppStartCounter { get; set; } = 0;

		//TODO: local groups

	}

}
