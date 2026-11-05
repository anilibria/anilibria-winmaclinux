export default {
	props: ['backToTopVisible', 'releasesList'],
	template: `
	<div v-if="releasesList" :class="{'selection-helper': true, 'selection-pulse-animation': releasesList.selectionModeEnabled }">
		{{ releasesList.selectionModeEnabled ? 'Нажмите ЛКМ для выделения релизов, ПКМ для выхода' : 'Нажмите ПКМ для перехода в режим выделения' }}
	</div>
	<div v-if="backToTopVisible" class="selection-backtotop-helper flex-row flex-base-center flex-other-center" @click="backToTop()">
		<img src="/static/icons/commonbuttons/arrowup.svg" width="20" height="20" />
	</div>
	`,
	setup(props) {
        function backToTop() {
            props.releasesList.backToTop();
        }

		return {
            backToTop
		};
	}
};