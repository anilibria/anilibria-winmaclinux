import { ref, onMounted } from '/static/vue.js'
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
				<img class="common-bell-animation" :src="'/static/icons/releases/notifications.svg'" width="20" height="20" />
				<div class="simple-popup-box notification-list-container">
					<div v-for="notification in receivedNotifications" :class="{'common-notification': notification.type !== 'error', 'error-notification': notification.type === 'error'}">
						{{ notification.message }}
					</div>
				</div>
			</div>	
			<div v-if="synchronizationRunned" class="icon-button" :title="'Cинхронизация...' + synchronizationPercent + '%'">
				<img class="round-by-circle-animation" :src="'/static/icons/releases/synchronization.svg'" width="20" height="20" />
			</div>
			<div class="icon-button simple-popup-container" title="Полезные ссылки">
				<img src="/static/icons/commonbuttons/openinformation.svg" width="20" height="20" />
				<div class="simple-popup-box flex-column flex-base-center gap-2 main-info-links-container">
					<a href="https://t.me/desktopclientanilibria">Новости в телеграм</a>
					<a href="https://t.me/+Le_oNL4Tw745YWUy">Группа в телеграм</a>
					<a href="https://github.com/anilibria/anilibria-winmaclinux">Страница Github</a>
				</div>
			</div>
			<div class="icon-button" title="Сменить тему" @click.prevent="toggleTheme()">
				<img :src="'/static/icons/commonbuttons/changetheme.svg'" width="20" height="20" />
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

		function toggleTheme() {
			const currentTheme = document.documentElement.getAttribute('data-theme');
			const isLight = !currentTheme || currentTheme === 'light';
			document.documentElement.setAttribute('data-theme', isLight ? 'dark' : 'light');
		}

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
				if (!synchronizationRunned.value) synchronizationRunned.value = true;

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
			synchronizationPercent,
			toggleTheme
		};
	}
};