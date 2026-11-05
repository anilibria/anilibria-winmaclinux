namespace Aniliberty.Unfolded.Models.Releases {

	public record FranchisesModel {

		public List<string> Names { get; init; } = [];

		public Dictionary<int, int> Releases { get; init; } = new Dictionary<int, int>();

	}

}
