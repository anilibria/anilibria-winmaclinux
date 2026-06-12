namespace Aniliberty.Unfolded.Models.OriginalApi
{
    internal class ReleaseIdModel
    {

        public ReleaseMetaModel Meta { get; set; } = new ReleaseMetaModel();

        public IEnumerable<ReleaseDataIdModel> Data { get; set; } = Enumerable.Empty<ReleaseDataIdModel>();

    }
}
