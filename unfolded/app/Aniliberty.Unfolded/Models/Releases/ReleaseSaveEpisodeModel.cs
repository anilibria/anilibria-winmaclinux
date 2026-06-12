using Aniliberty.Unfolded.Models.OriginalApi;

namespace Aniliberty.Unfolded.Models.Releases
{

    internal record ReleaseSaveEpisodeModel
    {

        public int ReleaseId { get; init; }

        public IEnumerable<ReleaseEpisodeModel> Items { get; init; } = Enumerable.Empty<ReleaseEpisodeModel>();

    }

}
