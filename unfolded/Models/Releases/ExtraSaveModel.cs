namespace Aniliberty.Unfolded.Models.Releases
{

    internal record ExtraSaveModel
    {

        public List<ReleaseSaveModel> Releases { get; set; } = [];

        public List<ReleaseTorrentSaveModel> Torrents { get; set; } = [];

        public List<ReleaseSaveEpisodeModel> Videos { get; set; } = [];

    }

}
