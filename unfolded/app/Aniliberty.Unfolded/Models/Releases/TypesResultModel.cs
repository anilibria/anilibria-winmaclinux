using Aniliberty.Unfolded.Models.OriginalApi;

internal record TypesResultModel {

    public IEnumerable<StringValueItem> AgeRatings { get; init; } = Enumerable.Empty<StringValueItem> ();

    public IEnumerable<IntegerValueItem> Genres { get; init; } = Enumerable.Empty<IntegerValueItem> ();

    public IEnumerable<StringValueItem> Seasons { get; init; } = Enumerable.Empty<StringValueItem> ();

    public IEnumerable<StringValueItem> Types { get; init; } = Enumerable.Empty<StringValueItem> ();

}