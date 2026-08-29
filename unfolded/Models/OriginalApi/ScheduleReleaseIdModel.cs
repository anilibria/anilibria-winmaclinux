namespace Aniliberty.Unfolded.Models.OriginalApi
{

    public class ScheduleReleaseIdModel
    {

        public int Id { get; set; }

        public ReleaseDataModelPublishDay PublishDay { get; init; } = new ReleaseDataModelPublishDay();

    }

}