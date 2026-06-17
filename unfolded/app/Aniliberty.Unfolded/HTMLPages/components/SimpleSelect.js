import { ref } from '/static/vue.global.js'
import { getMainMenu, getUserData } from '/static/mainmenu.js'

export default {
	props: ['item','options'],
	emits: ['update:item'],
	template: `
<div class="simple-select-container">
	<select v-model="selectValue" @change="$emit('update:item', $event)" class="simple-select">
		<option v-for="option in options" :value="option.id" :key="option.id">
			{{option.title}}
		</option>
	</select>
</div>`,
	setup(props) {
		const selectValue = ref(props.item);

		return {
			selectValue
		};
	}
};