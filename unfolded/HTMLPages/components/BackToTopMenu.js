export default {
	props: ['backToTopVisible', 'releasesList'],
	template: `
    <div v-if="releasesList">
		<div :class="{'selection-helper': true, 'selection-pulse-animation': releasesList.selectionModeEnabled }">
			{{ releasesList.selectionModeEnabled ? 'Нажмите ЛКМ для выделения релизов, ПКМ для выхода' : 'Нажмите ПКМ для перехода в режим выделения' }}
		</div>
		<div v-if="backToTopVisible" class="selection-backtotop-helper flex-row flex-base-center flex-other-center" @click="backToTop()">
			<img src="/static/icons/commonbuttons/arrowup.svg" width="20" height="20" />
		</div>
    </div>
	`,
	setup() {
        function backToTop() {
            releasesList.value.backToTop();
        }

		return {
            backToTop
		};
	}
};