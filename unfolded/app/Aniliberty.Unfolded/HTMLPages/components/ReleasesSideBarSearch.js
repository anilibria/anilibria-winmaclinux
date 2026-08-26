import { ref } from '/static/vue.js'
import { getReleaseDictionaries } from '/static/unfoldapi.js'
import SimpleSwitch from '/static/components/SimpleSwitch.js'
import MultiSelect from '/static/libs/multiselect.mjs'

export default {
	props: ['filterModel','releasesList'],
    components: {
        'multi-select': MultiSelect,
        'simple-switch': SimpleSwitch,
    },
	template: `
<div class="simple-panel-icon-button simple-popup-container"
    title="Добавить фильтры для фильтрации по разным полям релиза">
    <div class="full-width-height flex-row flex-base-center flex-other-center">
        <img :src="'/static/icons/releases/search.svg'" width="30" height="30" />
    </div>
    <div class="simple-popup-box releaseslist-rightmenu">
        <div class="releaseslist-rightmenu-search flex-column">
            <div class="flex-column gap-2">
                <span>Команда</span>
                <div class="flex-row flex-base-center gap-4">
                    <multi-select class="releaseslist-search-multiselect flex-1" v-model="filterModel.team" :options="releaseDictionaries.teams" mode="tags" searchable @select="loadReleases()" @deselect="loadReleases()" @clear="loadReleases()">
                    </multi-select>
                    <div class="no-text-wrap" style="margin-left: 4px; margin-right: 4px;">
                        <span>ИЛИ/И</span>
                    </div>
                    <simple-switch v-model:state="filterModel.teamOr" @changed="loadReleases()">
                    </simple-switch>
                </div>
            </div>
            <div class="flex-column gap-2">
                <span>Жанры</span>
                <div class="flex-row flex-base-center gap-4">
                    <multi-select class="releaseslist-search-multiselect flex-1" v-model="filterModel.genres" :options="releaseDictionaries.genres" mode="tags" searchable @select="loadReleases()" @deselect="loadReleases()" @clear="loadReleases()">
                    </multi-select>
                    <div class="no-text-wrap" style="margin-left: 4px; margin-right: 4px;">
                        <span>ИЛИ/И</span>
                    </div>
                    <simple-switch v-model:state="filterModel.genresOr" @changed="loadReleases()">
                    </simple-switch>
                </div>
            </div>
            <div class="flex-column gap-2">
                <span>Года</span>
                <div class="flex-row flex-base-center gap-4">
                    <multi-select class="releaseslist-search-multiselect flex-1" v-model="filterModel.years" :options="releaseDictionaries.years" mode="tags" searchable @select="loadReleases()" @deselect="loadReleases()" @clear="loadReleases()">
                    </multi-select>
                </div>
            </div>
            <div class="flex-column gap-2">
                <span>Сезоны</span>
                <div class="flex-row flex-base-center gap-4">
                    <multi-select class="releaseslist-search-multiselect flex-1" v-model="filterModel.seasons" :options="releaseDictionaries.seasons" mode="tags" searchable @select="loadReleases()" @deselect="loadReleases()" @clear="loadReleases()">
                    </multi-select>
                </div>
            </div>
            <div class="flex-column gap-2">
                <span>Статусы</span>
                <div class="flex-row flex-base-center gap-4">
                    <multi-select class="releaseslist-search-multiselect flex-1" v-model="filterModel.statuses" :options="releaseDictionaries.statuses" mode="tags" searchable @select="loadReleases()" @deselect="loadReleases()" @clear="loadReleases()">
                    </multi-select>
                </div>
            </div>
            <div class="flex-column gap-2">
                <span>Дни в расписании</span>
                <div class="flex-row flex-base-center gap-4">
                    <multi-select class="releaseslist-search-multiselect flex-1" v-model="filterModel.scheduleDays" :options="releaseDictionaries.scheduleDays" mode="tags" searchable @select="loadReleases()" @deselect="loadReleases()" @clear="loadReleases()">
                    </multi-select>
                </div>
            </div>
            <div class="flex-column gap-2">
                <span>Описание</span>
                <input v-model="filterModel.description" type="text" class="simple-input" placeholder="Описание" />
            </div>
            <div class="flex-column gap-2">
                <span>Тип</span>
                <input v-model="filterModel.type" type="text" class="simple-input" placeholder="Тип" />
            </div>
            <div class="flex-row flex-base-center gap-8" style="height: 40px;">
                <span>В избранном</span>
                <simple-switch v-model:state="filterModel.inFavorites" @changed="loadReleases()">
                </simple-switch>
                <span>Часть франшизы</span>
                <simple-switch v-model:state="filterModel.partOfReleases" @changed="loadReleases()">
                </simple-switch>
            </div>
        </div>
    </div>
</div>
`,
	setup(props) {
        const releaseDictionaries = ref({
            genres: [],
            teams: [],
            statuses: [],
            years: [],
            seasons: [],
            scheduleDays: [
                { label: 'Понедельник', value: 1 },
                { label: 'Вторник', value: 2 },
                { label: 'Среда', value: 3 },
                { label: 'Четверг', value: 4 },
                { label: 'Пятница', value: 5 },
                { label: 'Суббота', value: 6 },
                { label: 'Воскресенье', value: 7 }
            ]
        });

        async function loadAllData() {
            const dictionaries = await getReleaseDictionaries();
            releaseDictionaries.value.genres = dictionaries.genres;
            releaseDictionaries.value.teams = dictionaries.teams;
            releaseDictionaries.value.statuses = dictionaries.statuses;
            releaseDictionaries.value.years = dictionaries.years;
            releaseDictionaries.value.seasons = dictionaries.seasons;
        }

        async function loadReleases() {
            await props.releasesList.loadReleases();
        }

        loadAllData();

		return {
            releaseDictionaries,
            loadReleases
		};
	}
};