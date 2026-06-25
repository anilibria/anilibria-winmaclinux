import { ref, onMounted } from '/static/vue.global.js'
import { synchronizeFirstStart, synchronizeUser, synchronizeReleases } from '/static/unfoldapi.js'

export default {
	props: ['mainMenuVisible', 'title'],
	emits: ['update:mainMenuVisible'],
	template: `
		<div class="page-upper-header">
			<div class="icon-button" @click="$emit('update:mainMenuVisible', mainMenuVisible ? false : true)" title="Открыть меню приложения">
				<img src="/static/icons/commonbuttons/opendrawer.svg" width="20" height="20" />
			</div>
			<div class="page-header">{{title}}</div>
			<div class="icon-button" title="Полезные ссылки">
				<img src="/static/icons/commonbuttons/openinformation.svg" width="20" height="20" />
			</div>
			<div class="icon-button" title="Вернуться назад в истории переходов" @click="history.back();">
				<img :src="'/static/icons/commonbuttons/' + (history.length > 1 ? 'navigationback' : 'navigationbackdisabled') + '.svg'" width="20" height="20" />
			</div>
		</div>
	`,
	setup(props) {

		onMounted(async () => {
			const firstStart = await synchronizeFirstStart();
			if (firstStart) {
				await synchronizeUser();
				await synchronizeReleases();
			}
		});

		return {
			history
		};
	}
};