import { ref, onMounted } from '/static/vue.js'
import {
	synchronizeUser, synchronizeReleases, synchronizeFullStatus,
	webSocketObserver, synchronizeStatus, getRelease
} from '/static/unfoldapi.js'

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

		function isLocalStorageAvailable() {
			try {
				const storage = window.localStorage;
				const x = "__storage_test__";
				storage.setItem(x, x);
				storage.removeItem(x);
				return true;
			} catch (e) {
				return (
				e instanceof DOMException &&
				e.name === "QuotaExceededError" &&
				// Acknowledge QuotaExceededError only if there's already something stored
				window.localStorage &&
				window.localStorage.length !== 0
				);
			}
		}

		function restoreTheme() {
			if (isLocalStorageAvailable()) {
				const savedTheme = localStorage.getItem('unfoldtheme');
				if (savedTheme) document.documentElement.setAttribute('data-theme', savedTheme);
			}
		}

		function toggleTheme() {
			const currentTheme = document.documentElement.getAttribute('data-theme');
			const isLight = !currentTheme || currentTheme === 'light';
			document.documentElement.setAttribute('data-theme', isLight ? 'dark' : 'light');
			if (isLocalStorageAvailable()) {
				localStorage.setItem('unfoldtheme', isLight ? 'dark' : 'light');
			}
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

		async function torrentHandler(message) {
			const parts = message.split('-');
			if (parts.length !== 2) return;

			const action = parts[0];
			const releaseId = parseInt(parts[1]);
			const release = await getRelease(releaseId);
			let resultMessage = '';
			switch (action) {
				case 'startmeta':
					resultMessage = 'Старт закачки метаданных: ' + release.title;
					break;
				case 'startdownload':
					resultMessage = 'Начало скачивания: ' + release.title;
					break;
				case 'enddownload':
					resultMessage = 'Cкачивание завершено: ' + release.title;
					break;
				case 'failmetadata':
					resultMessage = 'Не удалось получить метаданные релиза: ' + release.title;
					break;
				case 'remove':
					resultMessage = 'Торрент удален для релиза: ' + release.title;
					break;
				case 'removeall':
					resultMessage = 'Все торренты удалены';
					break;
			}
			if (resultMessage) receivedNotifications.value.unshift({ type: 'common', message: resultMessage });
		}

		webSocketObserver().synchronization = synchronizedHandler;
		webSocketObserver().notification = notificationHandler;
		webSocketObserver().torrent = torrentHandler;

		onMounted(async () => {
			const fullStatus = await synchronizeFullStatus();
			if (fullStatus.status) synchronizationRunned.value = true;

			if (fullStatus.firstStart && !fullStatus.status) {
				await synchronizeUser();
				await synchronizeReleases();
			}

			restoreTheme();
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