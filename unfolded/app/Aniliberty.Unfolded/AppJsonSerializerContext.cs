using Aniliberty.Unfolded.Models.CacheModels;
using Aniliberty.Unfolded.Models.MainMenus;
using Aniliberty.Unfolded.Models.OriginalApi;
using Aniliberty.Unfolded.Models.Releases;
using Aniliberty.Unfolded.Models.Settings;
using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded
{

	[JsonSerializable(typeof(ReleasesListFiltersModel))]
	[JsonSerializable(typeof(ReleaseSaveModel))]
	[JsonSerializable(typeof(MetadataModel))]
	[JsonSerializable(typeof(TypesResultModel))]
	[JsonSerializable(typeof(List<ReleaseTorrentsSaveModel>))]
	[JsonSerializable(typeof(List<ReleaseSaveEpisodeModel>))]
	[JsonSerializable(typeof(List<ReleaseSaveModel>))]
	[JsonSerializable(typeof(ReleaseIdModel))]
	[JsonSerializable(typeof(IEnumerable<ScheduleReleaseModel>))]
	[JsonSerializable(typeof(IEnumerable<StringValueItem>))]
	[JsonSerializable(typeof(IEnumerable<IntegerValueItem>))]
	[JsonSerializable(typeof(IEnumerable<FranchiseModel>))]
	[JsonSerializable(typeof(FranchiseReleasesModel))]
	[JsonSerializable(typeof(ReleaseDataFullCollectionModel))]
	[JsonSerializable(typeof(MainMenuUserModel))]
	[JsonSerializable(typeof(IEnumerable<MainMenuModel>))]
	[JsonSerializable(typeof(IEnumerable<ReleaseSaveModel>))]
	[JsonSerializable(typeof(UserProfileModel))]
	[JsonSerializable(typeof(IEnumerable<int>))]
	[JsonSerializable(typeof(IEnumerable<IEnumerable<object>>))]
	[JsonSerializable(typeof(MarksModel))]
	[JsonSerializable(typeof(LoginPassAuthModel))]
	[JsonSerializable(typeof(LoginPassAuthResponseModel))]
	[JsonSerializable(typeof(UserCollections))]
	[JsonSerializable(typeof(IEnumerable<ReleaseDisplayEpisodeModel>))]
	[JsonSerializable(typeof(ApplicationSettingsModel))]
	[JsonSerializable(typeof(ApplicationSettingsReleasesModel))]
	[JsonSerializable(typeof(ApplicationSettingsVideoplayerModel))]
	[JsonSerializable(typeof(IEnumerable<ReleaseDisplayTorrentModel>))]
	[JsonSerializable(typeof(List<ReleaseSeriesSaveModel>))]
	[JsonSerializable(typeof(List<FavoritesEditModel>))]
	[JsonSerializable(typeof(IEnumerable<int>))]
	[JsonSerializable(typeof(DisplayFranchiseModel))]
	[JsonSerializable(typeof(List<SeenMarkEditModel>))]
	[JsonSerializable(typeof(string[]))]
	[JsonSourceGenerationOptions(PropertyNamingPolicy = JsonKnownNamingPolicy.CamelCase)]
	internal partial class AppJsonSerializerContext : JsonSerializerContext
	{

	}

}
