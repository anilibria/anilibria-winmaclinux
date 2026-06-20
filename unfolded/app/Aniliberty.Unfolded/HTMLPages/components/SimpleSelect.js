import { ref, watch } from '/static/vue.global.js'
import { getMainMenu, getUserData } from '/static/mainmenu.js'

export default {
	props: ['item','options'],
	emits: ['update:item', 'selected'],
	template: `
<div class="simple-select-container">
	<select v-model="selectValue" class="simple-select">
		<option v-for="option in options" :value="option.id" :key="option.id">
			{{option.title}}
		</option>
	</select>
</div>`,
	setup(props, context) {
		const selectValue = ref(props.item);

		watch(
			selectValue,
			(newValue) => {
				context.emit('update:item', newValue);
				context.emit('selected', newValue);
			}
		);

		return {
			selectValue
		};
	}
};