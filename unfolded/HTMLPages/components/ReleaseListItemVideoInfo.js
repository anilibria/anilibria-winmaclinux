export default {
	props: ['release', 'openLinkMode', 'seensCount', 'videomode'],
	template: `
<img
    class="clickable-image"
    src="/static/icons/releases/online.svg"
    width="20"
    height="20"
    @click.stop="openInVideoPlayer(release.id)"
    title="Открыть релиз в видеоплеере"
    loading="lazy"
/>
<span>{{ release.countVideos }}{{ seensCount > 0 ? '/' + seensCount : '' }}</span>`,
	setup(props) {

        function openInVideoPlayer(id) {
            const mode = props.videomode ? `mode=${props.videomode}&` : '';

            if (props.openLinkMode) {
                window.open(`/static/videoplayer.html?${mode}id=` + id, '_blank');
            } else {
                window.location.href = `/static/videoplayer.html?${mode}id=` + id;
            }
        }

		return {
            openInVideoPlayer
		};
	}
};