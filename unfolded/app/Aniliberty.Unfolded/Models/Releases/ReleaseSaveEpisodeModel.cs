using Aniliberty.Unfolded.Models.OriginalApi;

namespace Aniliberty.Unfolded.Models.Releases
{

    internal class ReleaseSaveEpisodeModel
    {

        public int ReleaseId { get; set; }

        public IEnumerable<ReleaseSaveEpisodeItemModel> Items { get; set; } = Enumerable.Empty<ReleaseSaveEpisodeItemModel>();

    }

	internal record ReleaseSaveEpisodeItemModel
	{

		public string Id { get; set; } = "";

		public string Name { get; set; } = "";

		public decimal Ordinal { get; set; }

		public int SortOrder { get; set; }

		public string Hls480 { get; set; } = "";

		public string Hls720 { get; set; } = "";

		public string Hls1080 { get; set; } = "";

		public long Duration { get; set; }

		public string RutubeId { get; set; } = "";

		public string YoutubeId { get; set; } = "";

		public string UpdatedAt { get; set; } = "";

		public int? OpeningStart { get; set; }

		public int? OpeningEnd { get; set; }

		public int? EndingStart { get; set; }

		public int? EndingEnd { get; set; }

		public string Poster { get; set; } = "";

		internal void MapFromApiModel(ReleaseEpisodeModel episode)
		{
			Id = episode.Id;
			Name = episode.Name ?? (episode.NameEnglish ?? "");
			Ordinal = episode.Ordinal;
			SortOrder = episode.SortOrder;
			Hls480 = episode.Hls480;
			Hls720 = episode.Hls720;
			Hls1080 = episode.Hls1080;
			Duration = episode.Duration;
			RutubeId = episode.RutubeId;
			YoutubeId = episode.YoutubeId;
			UpdatedAt = episode.UpdatedAt;
			OpeningStart = episode.Opening?.Start;
			OpeningEnd = episode.Opening?.Stop;
			EndingStart = episode.Ending?.Start;
			EndingEnd = episode.Ending?.Stop;
			Poster = episode.Preview.Src;
		}

		internal static ReleaseSaveEpisodeItemModel CreateFromApi(ReleaseEpisodeModel episode)
		{
			var model = new ReleaseSaveEpisodeItemModel();
			model.MapFromApiModel(episode);
			return model;
		}

	}

}
