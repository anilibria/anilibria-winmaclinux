namespace Aniliberty.Unfolded.Models.OriginalApi
{
    internal record ReleaseDataFullModel : ReleaseDataModel
    {

        public IEnumerable<ReleaseEpisodeModel> Episodes { get; set; } = Enumerable.Empty<ReleaseEpisodeModel>();

        public IEnumerable<ReleaseMemberModel> Members { get; set; } = Enumerable.Empty<ReleaseMemberModel>();

        public IEnumerable<ReleaseTorrentModel> Torrents { get; set; } = Enumerable.Empty<ReleaseTorrentModel>();

    }
}
