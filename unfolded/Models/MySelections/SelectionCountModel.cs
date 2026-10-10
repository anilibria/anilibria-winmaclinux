namespace Aniliberty.Unfolded.Models.MySelections
{

	public record SelectionCountModel : CountModel
	{

		public IEnumerable<SelectionCountReleaseModel> Releases { get; init; } = Enumerable.Empty<SelectionCountReleaseModel>();
	
	}

}
