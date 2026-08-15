using Aniliberty.Unfolded.Models.OriginalApi;
using Aniliberty.Unfolded.Routes;

namespace Aniliberty.Unfolded.Models.Releases
{

	public class ReleaseSaveModel
	{

		public string Announce { get; set; } = "";

		public string Code { get; set; } = "";

		public int CountTorrents { get; set; }

		public int CountVideos { get; set; }

		public string Description { get; set; } = "";

		public IEnumerable<string> Genres { get; set; } = Enumerable.Empty<string>();

		public int Id { get; set; }

		public string OriginalName { get; set; } = "";

		public string Poster { get; set; } = "";

		public int Rating { get; set; }

		public string Season { get; set; } = ""; // need to remake on int

		public int Series { get; set; }

		public string Status { get; set; } = ""; // need to remake on int

		public long Timestamp { get; set; }

		public string Title { get; set; } = "";

		public string Type { get; set; } = "";  // need to remake on int

		public IEnumerable<string> Voices { get; set; } = Enumerable.Empty<string>();

		public IEnumerable<string> Team { get; set; } = Enumerable.Empty<string>();

		public IEnumerable<string> Codecs { get; set; } = Enumerable.Empty<string>();

		public int Year { get; set; }

		public bool IsOngoing { get; set; }

		public string AgeRating { get; set; } = "";

		public int? PublishDay { get; set; }

		internal void MapFromApiModel(ReleaseDataFullModel fullRelease, TypesResultModel types)
		{
			Id = fullRelease.Id;
			Announce = fullRelease.Notification ?? "";
			Code = fullRelease.Alias;
			CountVideos = fullRelease.Episodes?.Count() ?? 0;
			CountTorrents = fullRelease.Torrents?.Count() ?? 0;
			Description = fullRelease.Description ?? "";
			Timestamp = Synchronize.ParseDateTimeOffset(fullRelease.FreshAt);
			OriginalName = fullRelease.Name.English;
			Title = fullRelease.Name.Main;
			Rating = fullRelease.AddedInUsersFavorites ?? 0;
			Year = fullRelease.Year;
			Season = types.Seasons.FirstOrDefault(a => a.Value == fullRelease.Season.Value)?.Description ?? "Не указано";
			Status = fullRelease.IsInProduction ? "Сейчас в озвучке" : "Озвучка завершена";
			Series = fullRelease.EpisodesAreUnknown ? -1 : fullRelease.EpisodesTotal ?? 0;
			Poster = fullRelease.Poster?.Src ?? "";
			Type = types.Types.FirstOrDefault(a => a.Value == fullRelease.Type.Value)?.Description ?? fullRelease.Type.Value;
			Genres = fullRelease.Genres.Select(a => types.Genres.FirstOrDefault(b => b.Id == a.Id)?.Name ?? "").Where(a => !string.IsNullOrEmpty(a));
			IsOngoing = fullRelease.IsOngoing;
			AgeRating = types.AgeRatings.FirstOrDefault(a => a.Value == fullRelease.AgeRating.Value)?.Description ?? fullRelease.AgeRating.Value;
			Voices = fullRelease.Members != null ? fullRelease.Members.Where(a => a.Role.Value == "voicing").Select(a => a.Nickname) : [];
			Team = fullRelease.Members != null ? fullRelease.Members.OrderByDescending(a => a.Role.Value).Select(a => a.Nickname) : [];
			Codecs = fullRelease.Torrents?.Select(a => a.Codec?.Value ?? "").Where(a => a != "")?.Distinct().ToList() ?? [];
			PublishDay = fullRelease.IsInProduction ? fullRelease.PublishDay?.Value : null;
		}

	}

}
