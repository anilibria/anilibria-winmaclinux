import { ref, onMounted } from '/static/vue.global.js'
import { synchronizeFirstStart, synchronizeUser, synchronizeReleases, webSocketObserver, synchronizeStatus } from '/static/unfoldapi.js'

export default {
	props: ['mainMenuVisible', 'title'],
	emits: ['update:mainMenuVisible'],
	template: `
		<div class="page-upper-header">
			<div class="icon-button" @click="$emit('update:mainMenuVisible', mainMenuVisible ? false : true)" title="Открыть меню приложения">
				<img src="/static/icons/commonbuttons/opendrawer.svg" width="20" height="20" />
			</div>
			<div class="page-header">{{title}}</div>
			<div v-if="receivedNotifications.length" class="icon-button simple-popup-container" title="Уведомления">
				<img :src="'/static/icons/releases/notifications.svg'" width="20" height="20" />
				<div class="simple-popup-box notification-list-container">
					<div v-for="notification in receivedNotifications" :class="{'common-notification': notification.type !== 'error', 'error-notification': notification.type === 'error'}">
						{{ notification.message }}
					</div>
				</div>
			</div>
			<div v-if="synchronizationRunned" class="icon-button" :title="'Cинхронизация...' + synchronizationPercent + '%'">
				<img :src="'/static/icons/releases/synchronization.svg'" width="20" height="20" />
			</div>
			<div class="icon-button" title="Полезные ссылки">
				<img src="/static/icons/commonbuttons/openinformation.svg" width="20" height="20" />
			</div>
			<div class="icon-button" title="Вернуться назад в истории переходов" @click="history.back();">
				<img :src="'/static/icons/commonbuttons/' + (history.length > 1 ? 'navigationback' : 'navigationbackdisabled') + '.svg'" width="20" height="20" />
			</div>
		</div>
	`,
	setup(props) {
		const receivedNotifications = ref([]);
		const synchronizationRunned = ref(false);
		const synchronizationPercent = ref(0);

		function synchronizedHandler(message) {
			if (message === 'started') {
				synchronizationRunned.value = true;
				return;
			}
			if (message === 'completed') {
				synchronizationRunned.value = false;
				return;
			}
			if (message === 'uptodate') {
				synchronizationRunned.value = false;
				notificationHandler('common', 'Нет новых данных для синхронизации!');
				return;
			}
			if (message.indexOf('percent') === 0) {
				synchronizationPercent.value = parseInt(message.replace('percent', ''));
			}
		}

		function notificationHandler(type, message) {
			receivedNotifications.value.unshift({ type, message });
		}

		webSocketObserver().synchronization = synchronizedHandler;
		webSocketObserver().notification = notificationHandler;

		onMounted(async () => {
			const synchronizedStarted = await synchronizeStatus();
			if (synchronizedStarted) synchronizationRunned.value = true;

			const firstStart = await synchronizeFirstStart();
			if (firstStart) {
				await synchronizeUser();
				await synchronizeReleases();
			}
		});

		return {
			history,
			receivedNotifications,
			synchronizationRunned,
			synchronizationPercent
		};
	}
};