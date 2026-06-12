namespace Aniliberty.Unfolded.Models.OriginalApi {

    internal record ReleaseTorrentModel {

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

        public string UpdatedAt { get; init; } = "";

    }
}
