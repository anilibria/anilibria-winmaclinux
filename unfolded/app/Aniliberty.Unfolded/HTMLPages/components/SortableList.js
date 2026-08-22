import { ref } from '/static/vue.js'

export default {
	props: ['options'],
	emits: ['update:options','changed'],
	template: `
<div v-for="item in options">
    <div class="flex-row flex-base-center gap-2" @dragstart="dragStart(item, $event)" @drop="dropItem(item)" @dragover.prevent draggable="true">
        <div class="flex-1">{{ item }}</div>
        <img src='/static/icons/commonbuttons/drag.svg' width="20" height="20" />
    </div>
</div>`,
	setup(props, context) {
        const draggedItem = ref(null);

        function dragStart(item, element) {
            draggedItem.value = item;
            
            element.dataTransfer.effectAllowed = "move";
            element.dataTransfer.setData("text/plain", null);
        }

        function dropItem(item) {
            if (draggedItem.value === item) return;

            const newArray = [];
            let inverted = false;
            for (let element of props.options) {
                if (element === item) {
                    if (inverted) {
                        newArray.push(item);
                        newArray.push(draggedItem.value);
                    } else {
                        newArray.push(draggedItem.value);
                        newArray.push(item);
                    }
                    continue;
                }
                if (element === draggedItem.value) {
                    inverted = true;
                    continue;
                }

                newArray.push(element);
            }            

			context.emit('update:options', newArray);
			context.emit('changed');

            draggedItem.value = null;
        }

		return {
            dragStart,
            dropItem
		};
	}
};