namespace Aniliberty.Unfolded.Models.OriginalApi
{

    internal class FranchiseModel
    {

        public string Id { get; set; } = "";

        public string Name { get; set; } = "";

        public string NameEnglish { get; set; } = "";

        public decimal? Rating { get; set; } = null;

        public int? LastYear { get; set; }

        public int? FirstYear { get; set; }

        public int? TotalReleases { get; set; }

        public int? TotalEpisodes { get; set; }

        public string TotalDuration { get; set; } = "";

        public int? TotalDurationInSeconds { get; set; }

        public FranchiseImageModel Image { get; set; } = new FranchiseImageModel();

    }

}
