import { watch } from '/static/vue.js'
import SimpleSelect from '/static/components/SimpleSelect.js'
import { sortingFields, sections, historySections, seenMarkSections, setsSections } from '/static/releaseDictionaries.js'

export default {
	props: ['filterModel','releasesList'],
	emits: [],
    components: {
        'simple-select': SimpleSelect
    },
	template: `
<div class="page-subheader page-line-pair">
    <div class="flex-row flex-base-center" style="margin: 0 0 0 4px;">
        <simple-select v-model:item="filterModel.section"
                        :options="sections"
                        style="width: 130px;"
                        @selected="sectionChanged($event)">
        </simple-select>
        <div style="width: 4px;"></div>
        <simple-select v-if="filterModel.section === 6" v-model:item="filterModel.subsection"
                        :options="historySections"
                        style="width: 170px;"
                        @selected="subSectionChanged($event)">
        </simple-select>
        <simple-select v-if="filterModel.section === 7" v-model:item="filterModel.subsection"
                        :options="seenMarkSections"
                        style="width: 225px;"
                        @selected="subSectionChanged($event)">
        </simple-select>
        <simple-select v-if="filterModel.section === 8" v-model:item="filterModel.subsection"
                        :options="setsSections"
                        style="width: 220px;"
                        @selected="subSectionChanged($event)">
        </simple-select>
    </div>
    <div class="flex-row flex-base-center flex-other-center">
        <input v-model="filterModel.filter" type="text" class="simple-input" placeholder="Введите название релиза" />
    </div>
    <div class="flex-row flex-base-center" style="margin-right: 4px;">
        <simple-select v-model:item="filterModel.sortingField"
                        :options="sortingFields"
                        style="width: 170px; margin-right: 2px;"
                        @selected="changedSortingFields()">
        </simple-select>
        <div class="simple-icon-button"
                title="Направление сортировки поиска"
                @click="toggleSortingDescending()">
            <img :src="'/static/icons/releases/' + (filterModel.sortingDescending ? 'sortdescending' : 'sortascending') + '.svg'" width="20" height="20" />
        </div>
    </div>
</div>
`,
	setup(props) {
        let currentTimer = -1;

        function changedSortingFields() {
            props.releasesList.sortingChanged(props.filterModel.sortingField, props.filterModel.sortingDescending);
        }

        function sectionChanged() {
            props.releasesList.sectionChanged(props.filterModel.section);
        }

        function subSectionChanged() {
            props.releasesList.subSectionChanged(props.filterModel.subsection);
        }

        function toggleSortingDescending() {
            props.releasesList.sortingChanged(props.filterModel.sortingField, !props.filterModel.sortingDescending);
        }

        function loadReleaseAfterTimer() {
            if (currentTimer !== -1) clearTimeout(currentTimer);

            currentTimer = setTimeout(() => {
                props.releasesList.loadReleases()
            }, 1000);
        }

        watch(
			() => props.filterModel.filter,
			() => { loadReleaseAfterTimer() }
		);


		return {
            sortingFields,
            sections,
            historySections,
            seenMarkSections,
            setsSections,
            sectionChanged,
            subSectionChanged,
            changedSortingFields,
            toggleSortingDescending
		};
	}
};