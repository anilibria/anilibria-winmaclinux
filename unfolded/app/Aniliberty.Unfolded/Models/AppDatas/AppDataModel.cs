namespace Aniliberty.Unfolded.Models.AppDatas
{

	internal class AppDataModel
	{

		public List<int> HidedReleases { get; set; } = Enumerable.Empty<int>().ToList();

		public List<int> Cinemahall { get; set; } = Enumerable.Empty<int>().ToList();

		//TODO: local groups

	}

}
