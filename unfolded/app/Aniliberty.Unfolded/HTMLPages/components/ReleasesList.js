import { ref, watch, useTemplateRef, onMounted } from '/static/vue.js'
import {
	getRelasesByFilter, getUserMarks, getPageSettings,
    getReleaseNotifications, getCinemahallReleases
} from '/static/unfoldapi.js'

export default {
	props: ['releasesselected', 'allfilters', 'itemWidth', 'itemHeight', 'itemGap', 'leftPanelWidth', 'backtotopvisible'],
	emits: ['openrelease', 'update:releasesselected', 'update:allfilters', 'update:backtotopvisible'],
	template: `
<div ref="releasesContainer" class="releaseslist-releases-container" @click.right.prevent="toggleSelectionMode()">
    <div v-if="releasesGroups.length" class="releaseslist-release-item-group" v-for="(releasesGroup, index) in releasesGroups" :key="index">
        <div v-if="(itemHeight * index) < scrollTopValue + scrollHeightValue"
            v-for="release in releasesGroup.releases"
            :key="release.id"
            @click.stop="toggleSelectionRelease(release.id)">
            <slot
                name="release"
                :settings="settings"
                :torrentSettings="torrentSettings"
                :isSelected="selectedReleases.includes(release.id)"
                :inCinemahall="cinemahallReleases.includes(release.id)"
                :inFavorites="userMarks.favorites.includes(release.id)"
                :seensCount="userMarks.fullSeenReleases.includes(release.id) ? release.countVideos : (userMarks.seenSeries[release.id] ? userMarks.seenSeries[release.id] : 0)"
                :release="release"
                >
            </slot>
        </div>
    </div>
    <div v-if="!releasesGroups.length" class="releaseslist-empty-search flex-row flex-base-center flex-other-center">
        <div>
            <slot name="empty">
                По текущему фильтру ничего не найдено.<br>
                Попробуйте указать другие фильтры или раздел и повторить поиск.
            </slot>
        </div>
    </div>    
</div>`,
	setup(props, context) {
		const releasesContainer = useTemplateRef('releasesContainer');
        
        // filters
        const filterModel = ref({
            filter: "",
            type: "",
            description: "",
            team: [],
            teamOr: null,
            genres: [],
            genresOr: null,
            years: [],
            seasons: [],
            statuses: [],
            scheduleDays: [],
            inFavorites: null,
            partOfReleases: null,
            seenStatus: null,
            sortingField: 0,
            sortingDescending: true,
            section: 0,
            subsection: -1
        });

        // release selection
        const selectedReleases = ref([]);
        const selectionModeEnabled = ref(false);
        
        // releases items and display options
        const releasesGroups = ref([]);
        const releasesCount = ref(0);
        let relasesInGroup = 1;
        const widthItem = (props.itemWidth || 480) + (props.itemGap || 10);
        const leftPanelWidth = (props.leftPanelWidth || 40);

        // user data
		const userMarks = ref({ favorites: [], fullSeenReleases: [], seenSeries: {} });
    	const torrentSettings = ref({});
        const settings = ref({ startedSection: 0, startedSubSection: -1, notificationMode: 0, openLinkMode: 0 });
		const cinemahallReleases = ref([]);

        // synchronize data
        const notificationMessage = ref('');

        // virtualize list
        const scrollTopValue = ref(0);
        const scrollHeightValue = ref(0);

        //////
        // Sections and selection related
        function adjustSortingBySection(section) {
            switch (section) {
                case 0: // all
                case 1: // favorite
                case 2: // notification
                case 4: // aphabet
                case 7: // seen
                case 8: // collections
                    filterModel.value.sortingField = 0;
                    filterModel.value.sortingDescending = true;
                    break;
                case 3: // schedule
                    filterModel.value.sortingField = 1;
                    filterModel.value.sortingDescending = false;
                    break;
                case 6: // history
                    filterModel.value.sortingField = 13;
                    filterModel.value.sortingDescending = false;
                    break;
            }

            context.emit('update:allfilters', filterModel.value);
        }

        function sortingChanged(field, descending) {
            filterModel.value.sortingField = field;
            filterModel.value.sortingDescending = descending;
            context.emit('update:allfilters', filterModel.value);
            loadReleases();
        }

        function sectionChanged(newValue) {
            filterModel.value.section = newValue;
            filterModel.value.subSection = -1;

            disableSelectionMode();
            releasesContainer.value.scrollTo({ top: 0, behavior: "instant" });            

            adjustSortingBySection();

            loadReleases();
        }

        function subSectionChanged(newValue) {
            filterModel.value.subSection = newValue;

            disableSelectionMode();
            releasesContainer.value.scrollTo({ top: 0, behavior: "instant" });            

            adjustSortingBySection();

            loadReleases();
        }

        function toggleSelectionMode() {
            selectedReleases.value = [];
            selectionModeEnabled.value = !selectionModeEnabled.value;

            context.emit('update:releasesselected', []);
        }

        function disableSelectionMode() {
            selectedReleases.value = [];
            selectionModeEnabled.value = false;

            context.emit('update:releasesselected', []);
        }

        function toggleSelectionRelease(id) {
            if (!selectionModeEnabled.value) {
                context.emit('openrelease', id);
                return;
            }

            if (selectedReleases.value.includes(id)) {
                selectedReleases.value = selectedReleases.value.filter(a => a !== id);
            } else {
                selectedReleases.value.push(id);
            }

            context.emit('update:releasesselected', selectedReleases.value);
        }

        //////
        // Load and prepare releases for display

        // reload releases by filters/sortings etc
        async function loadReleases() {
            const releases = await getRelasesByFilter(filterModel.value);

            recalculateReleaseGroups(releases);
        }

        async function refreshUserMarks() {
            userMarks.value = await getUserMarks();
        }

        async function refreshCinemahall() {
            cinemahallReleases.value = await getCinemahallReleases();
        }

        // load after load page all related data in one row
        async function loadAllData() {
            userMarks.value = await getUserMarks();
            settings.value = await getPageSettings('releases');
            torrentSettings.value = await getPageSettings('torrent');
            cinemahallReleases.value = await getCinemahallReleases();
            notificationMessage.value = await getReleaseNotifications();

            if (props.allfilters) filterModel.value = Object.assign(filterModel.value, props.allfilters);

            filterModel.value.section = settings.value.startedSection;
            filterModel.value.subSection = settings.value.startedSubSection;
            adjustSortingBySection();
            await loadReleases();
        }

        function getReleasesGroupCount() {
            relasesInGroup = Math.floor((window.innerWidth - leftPanelWidth) / widthItem);
            if (relasesInGroup === 0) return 1;
            return relasesInGroup;
        }

        function recalculateReleaseGroups(releases) {
            relasesInGroup = getReleasesGroupCount();
            releasesCount.value = releases.length;

            const result = [];
            let iterator = 0;
            let currentGroup = [];
            for (const release of releases) {
                if (iterator === relasesInGroup) {
                    result.push({ releases: [...currentGroup] });
                    currentGroup = [];
                    iterator = 0;
                }
                currentGroup.push(release);
                iterator++;
            }
            if (currentGroup.length) result.push({ releases: currentGroup });

            releasesGroups.value = result;
        }

        function backToTop() {
            releasesContainer.value.scrollTo({ top: 0, behavior: 'smooth' });
        }        

        loadAllData();

        window.addEventListener('resize', () => {
            const newRelasesInGroup = getReleasesGroupCount();
            if (newRelasesInGroup !== relasesInGroup.value) {
                const allReleases = releasesGroups.value.map(a => a.releases).flat()
                releasesGroups.value = [];
                recalculateReleaseGroups(allReleases);
            }
            scrollHeightValue.value = releasesContainer.value.clientHeight;
        });

        onMounted(() => {
            releasesContainer.value.addEventListener('scroll', () => {
                scrollTopValue.value = releasesContainer.value.scrollTop;
                context.emit('update:backtotopvisible', releasesContainer.value.scrollTop > 0);
            });

            setTimeout(() => {
                scrollHeightValue.value = releasesContainer.value.clientHeight;
                context.emit('update:allfilters', filterModel.value)
            });
        });

		return {
			releasesContainer,
            selectedReleases,
            selectionModeEnabled,
            releasesGroups,
            scrollTopValue,
            scrollHeightValue,
            userMarks,
            cinemahallReleases,
            settings,
            torrentSettings,
            sectionChanged,
            subSectionChanged,
            toggleSelectionRelease,
            backToTop,
            sortingChanged,
            toggleSelectionMode,
            loadReleases,
            refreshUserMarks,
            refreshCinemahall,
            disableSelectionMode
		};
	}
};