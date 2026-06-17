import { ref } from '/static/vue.global.js'
import { getMainMenu, getUserData } from '/static/mainmenu.js'

export default {
	props: ['mainMenuVisible'],
	emits: ['update:mainMenuVisible'],
	template: `
	<div class="main-menu-blackwall" @click="$emit('update:mainMenuVisible', false)" :style="{'visibility': (mainMenuVisible ? 'visible' : 'hidden')}">
		<div class="main-menu" :style="{'visibility': (mainMenuVisible ? 'visible' : 'hidden'), 'transform': (mainMenuVisible ? 'translateX(0px)': 'translateX(-300px)')}">
			<div v-if="userData && userData.name" class="main-menu-user">
				<img :src="userData.avatar" />
				<span>Вы авторизованы как:<br>{{userData.name}}</span>
				<img src="/static/icons/mainmenu/logout.svg" width="30" height="30" />
			</div>
			<div class="main-menu-items">
				<div v-for="item in mainMenu">
					<div class="main-menu-item">
						<img width="30" height="30" :src="'/static/icons/mainmenu/' + item.icon + '.svg'" />
						<span>{{item.name}}</span>
					</div>
				</div>
			</div>
			<div class="main-menu-logo">
				<div class="icon"></div>
				<span class="version">Aniliberty.Unfolded<br>версия 0.0.0</span>
			</div>
		</div>
	</div>
	`,
	setup(props) {
		const mainMenu = ref(getMainMenu());
		const userData = ref(getUserData());

		return {
			mainMenu,
			userData
		};
	}
};