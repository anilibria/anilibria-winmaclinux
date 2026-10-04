namespace Aniliberty.Unfolded.Models.MySelections
{

	public record CountModel
	{

		public int Count { get; set; }

		public string Name { get; set; } = "";

		public SelectionType Type { get; set; }

	}

}
