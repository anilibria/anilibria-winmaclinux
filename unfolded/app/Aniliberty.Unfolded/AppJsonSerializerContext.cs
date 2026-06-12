using Aniliberty.Unfolded.Models.OriginalApi;
using Aniliberty.Unfolded.Models.Releases;
using System.Text.Json.Serialization;

namespace Aniliberty.Unfolded
{

	[JsonSerializable(typeof(ReleasesListFiltersModel))]
	[JsonSerializable(typeof(ReleaseSaveModel))]
	[JsonSerializable(typeof(MetadataModel))]
	[JsonSerializable(typeof(TypesResultModel))]
	[JsonSerializable(typeof(List<ReleaseTorrentSaveModel>))]
	[JsonSerializable(typeof(List<ReleaseSaveEpisodeModel>))]
	[JsonSerializable(typeof(List<ReleaseSaveModel>))]
	[JsonSerializable(typeof(ReleaseIdModel))]
	[JsonSerializable(typeof(IEnumerable<ScheduleReleaseModel>))]
	[JsonSerializable(typeof(IEnumerable<StringValueItem>))]
	[JsonSerializable(typeof(IEnumerable<IntegerValueItem>))]
	[JsonSerializable(typeof(IEnumerable<FranchiseModel>))]
	[JsonSerializable(typeof(FranchiseReleasesModel))]
	[JsonSerializable(typeof(ReleaseDataFullCollectionModel))]
	internal partial class AppJsonSerializerContext : JsonSerializerContext
	{

	}

}
