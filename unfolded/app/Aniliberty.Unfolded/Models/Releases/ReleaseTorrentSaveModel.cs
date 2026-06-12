using Aniliberty.Unfolded.Models.OriginalApi;

namespace Aniliberty.Unfolded.Models.Releases {

    internal record ReleaseTorrentSaveModel {

        public int ReleaseId { get; set; }

        public long Time { get; set; }

        public string Hash { get; init; } = "";

        public int Id { get; init; }

        public string Magnet { get; init; } = "";

        public string Filename { get; init; } = "";

        public string Description { get; init; } = "";

        public StringValueItem Quality { get; init; } = new StringValueItem ();

        public StringValueItem Codec { get; init; } = new StringValueItem ();

        public StringValueItem Type { get; init; } = new StringValueItem ();

        public long Size { get; init; }

        public int Seeders { get; init; }

    }

}
