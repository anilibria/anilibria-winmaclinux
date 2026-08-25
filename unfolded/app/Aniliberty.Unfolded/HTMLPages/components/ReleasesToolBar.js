import { watch } from '/static/vue.js'
import SimpleSelect from '/static/components/SimpleSelect.js'

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

        const sortingFields = [
            {
                title: "Обновлению",
                id: 0
            },
            {
                title: "Расписанию",
                id: 1
            },
            {
                title: "Имени",
                id: 2
            },
            {
                title: "Году",
                id: 3
            },
            {
                title: "Рейтингу",
                id: 4
            },
            {
                title: "Статусу",
                id: 5
            },
            {
                title: "Ориг. имени",
                id: 6
            },
            {
                title: "Истории открытия",
                id: 7
            },
            {
                title: "Истории просмотра",
                id: 8
            },
            {
                title: "Сезону",
                id: 9
            },
            {
                title: "Избранности",
                id: 10
            },
            {
                title: "Просмотренности",
                id: 11
            },
            {
                title: "Части серии",
                id: 12
            },
            {
                title: "Истории",
                id: 13
            },
        ];

        const sections = [
            {
                title: "Все релизы",
                id: 0
            },
            {
                title: "Избранное",
                id: 1
            },
            {
                title: "Уведомления",
                id: 2
            },
            {
                title: "Расписание",
                id: 3
            },
            {
                title: "Алфавит",
                id: 4
            },
            {
                title: "История",
                id: 6
            },
            {
                title: "Группы",
                id: 5
            },
            {
                title: "Просмотр",
                id: 7
            },
            {
                title: "Подборки",
                id: 8
            }
        ];

        const historySections = [
            {
                title: "Не выбрано",
                id: -1
            },
            {
                title: "История",
                id: 40
            },
            {
                title: "История просмотра",
                id: 41
            }
        ];

        const seenMarkSections = [
            {
                title: "Не выбрано",
                id: -1
            },
            {
                title: "Просмотренные",
                id: 42
            },
            {
                title: "Просматриваемые",
                id: 43
            },
            {
                title: "Не просмотренные",
                id: 44
            },
            {
                title: "Скрытые релизы",
                id: 45
            },
            {
                title: "Просмотренные до конца",
                id: 46
            },
            {
                title: "Просмотренные не до конца",
                id: 47
            }
        ];

        const setsSections = [
            {
                title: "Не выбрано",
                id: -1
            },
            {
                title: "Фильмы",
                id: 48
            },
            {
                title: "Самые популярные",
                id: 49
            },
            {
                title: "Часть серии",
                id: 50
            },
            {
                title: "Завершенные",
                id: 51
            },
            {
                title: "Популярное в текущем году",
                id: 52
            },
            {
                title: "В кинозале",
                id: 53
            },
            {
                title: "Текущий сезон",
                id: 54
            },
            {
                title: "Не текущий сезон",
                id: 55
            }
        ];        

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