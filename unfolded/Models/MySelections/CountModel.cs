namespace Aniliberty.Unfolded.Models.MySelections
{

	public record CountModel
	{

		public string Id { get; set; } = "";

		public int Count { get; set; }

		public string Title { get; set; } = "";

		public SelectionType Type { get; set; }

	}

}
