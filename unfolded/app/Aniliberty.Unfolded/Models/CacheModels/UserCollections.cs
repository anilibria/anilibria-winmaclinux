namespace Aniliberty.Unfolded.Models.CacheModels
{
	public class UserCollections
	{

		public IEnumerable<int> CloudFavorites { get; set; } = Enumerable.Empty<int>();

		public IEnumerable<int> LocalFavorites { get; set; } = Enumerable.Empty<int>();

		public IEnumerable<string> SeenEpisodes { get; set; } = Enumerable.Empty<string>();

	}
}
