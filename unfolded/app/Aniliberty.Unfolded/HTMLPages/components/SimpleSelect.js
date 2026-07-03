import { ref, watch } from '/static/vue.global.js'
import { getMainMenu, getUserData } from '/static/mainmenu.js'

export default {
	props: ['item','options'],
	emits: ['update:item', 'selected'],
	template: `
<div class="simple-select-container">
	<select v-model="selectValue" class="simple-select" @change="changed($event, selectValue)">
		<option v-for="option in options" :value="option.id" :key="option.id">
			{{option.title}}
		</option>
	</select>
</div>`,
	setup(props, context) {
		const selectValue = ref(props.item);

		watch(
			() => props.item,
			(newValue) => {
				selectValue.value = newValue;
			}
		);

		function changed($event) {
			//setTimeout need because on that moment when these function is called selectedValue yet not updated
			setTimeout(() => {
				context.emit('update:item', selectValue.value);
				context.emit('selected', selectValue.value);
			})
		}

		return {
			selectValue,
			changed
		};
	}
};