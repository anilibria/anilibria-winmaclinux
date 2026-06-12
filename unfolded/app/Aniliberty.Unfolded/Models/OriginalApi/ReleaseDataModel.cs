namespace Aniliberty.Unfolded.Models.OriginalApi
{

    public record ReleaseDataModel
    {

        public int Id { get; init; }

        public int Year { get; init; }

        public string Alias { get; init; } = "";

        public string FreshAt { get; init; } = "";

        public bool IsOngoing { get; init; }

        public string Description { get; init; } = "";

        public int? EpisodesTotal { get; init; } = null;

        public bool EpisodesAreUnknown { get; init; }

        public bool IsBlockedByCopyrights { get; init; }

        public bool IsBlockedByGeo { get; init; }

        public int? AverageDurationOfEpisode { get; init; } = null;

        public int? AddedInUsersFavorites { get; init; } = null;

        public bool IsInProduction { get; init; }

        public string? Notification { get; set; }

        public ReleaseDataTypeModel Type { get; init; } = new ReleaseDataTypeModel();

        public ReleaseDataNameModel Name { get; init; } = new ReleaseDataNameModel();

        public ReleaseDataSeasonModel Season { get; init; } = new ReleaseDataSeasonModel();

        public ReleaseDataModelPoster Poster { get; init; } = new ReleaseDataModelPoster();

        public ReleaseDataModelAgeRating AgeRating { get; init; } = new ReleaseDataModelAgeRating();

        public ReleaseDataModelPublishDay PublishDay { get; init; } = new ReleaseDataModelPublishDay();

        public IEnumerable<ReleaseDataModelGenre> Genres { get; init; } = [];

    }

}