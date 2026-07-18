import { ref, watch } from '/static/vue.global.js'
import { getMainMenu, getUserData } from '/static/mainmenu.js'

export default {
	props: ['state'],
	emits: ['update:state','changed'],
	template: `
<div :class="{'simple-switch': true, 'simple-switch-on': state}" @click="click()">
</div>`,
	setup(props, context) {

		function click() {
			context.emit('update:state', !props.state);
			context.emit('changed');
		}

		return {
			click
		};
	}
};