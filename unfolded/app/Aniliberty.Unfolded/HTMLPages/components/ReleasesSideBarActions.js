import {
    addFavorites, removeFavorites,
    addReleasesSeenMark, removeReleasesSeenMark,
    addCinemahallReleases, removeCinemahallReleases,
    addHidedReleases, removeHidedReleases
} from '/static/unfoldapi.js'


export default {
	props: ['filterModel','selectedReleases','releasesList'],
	template: `
<div>
    <div v-if="selectedReleases.length"
            class="simple-panel-icon-button simple-popup-container"
            title="Добавить/удалить релизы в/из избранного">
        <div class="full-width-height flex-row flex-base-center flex-other-center">
            <img :src="'/static/icons/releases/favorite.svg'" width="30" height="30" />
        </div>
        <div class="simple-popup-box releases-rightmenu">
            <div class="releaseslist-rightmenu-actions flex-column">
                <a class="default-link" @click.prevent="addSelectedToFavorites()" href="">Добавить в избранное</a>
                <a class="default-link" @click.prevent="removeSelectedToFavorites()" href="">Удалить из избранного</a>
            </div>
        </div>
    </div>
    <div v-if="selectedReleases.length"
            class="simple-panel-icon-button simple-popup-container"
            title="Отметить релизы как просмотренные">
        <div class="full-width-height flex-row flex-base-center flex-other-center">
            <img :src="'/static/icons/releases/seen.svg'" width="30" height="30" />
        </div>
        <div class="simple-popup-box releases-rightmenu">
            <div class="releaseslist-rightmenu-seens flex-column">
                <a class="default-link" @click.prevent="addSelectedToSeens()" href="">Отметить как просмотренные</a>
                <a class="default-link" @click.prevent="removeSelectedToSeens()" href="">Отметить как не просмотренные</a>
                <a class="default-link" @click.prevent="addToHided()" href="">Скрыть релиз</a>
                <a v-if="filterModel.section === 7 && filterModel.subsection === 45" class="default-link" @click.prevent="removeFromHided()" href="">Сделать релиз видимым</a>
            </div>
        </div>
    </div>
    <div v-if="selectedReleases.length"
            class="simple-panel-icon-button simple-popup-container"
            title="Добавлять/удалять релизы в/из кинозала">
        <div class="full-width-height flex-row flex-base-center flex-other-center">
            <img :src="'/static/icons/mainmenu/cinemahall.svg'" width="30" height="30" />
        </div>
        <div class="simple-popup-box releases-rightmenu">
            <div class="releaseslist-rightmenu-actions flex-column">
                <a class="default-link" @click.prevent="addSelectedToCinemahall()" href="">Добавить в кинозал</a>
                <a class="default-link" @click.prevent="removeSelectedToCinemahall()" href="">Удалить из кинозала</a>
            </div>
        </div>
    </div>
</div>
`,
	setup(props) {
        async function addSelectedToFavorites() {
            await addFavorites(props.selectedReleases);
            props.releasesList.disableSelectionMode();
            props.releasesList.refreshUserMarks();
        }

        async function removeSelectedToFavorites() {
            await removeFavorites(props.selectedReleases);
            props.releasesList.disableSelectionMode();
            props.releasesList.refreshUserMarks();
        }

        async function addSelectedToSeens() {
            await addReleasesSeenMark(props.selectedReleases);
            props.releasesList.disableSelectionMode();
            props.releasesList.refreshUserMarks();
        }

        async function removeSelectedToSeens() {
            await removeReleasesSeenMark(props.selectedReleases);
            props.releasesList.disableSelectionMode();
            props.releasesList.refreshUserMarks();					
        }

        async function addSelectedToCinemahall() {
            await addCinemahallReleases(props.selectedReleases);
            props.releasesList.disableSelectionMode();
            props.releasesList.refreshCinemahall();
        }

        async function removeSelectedToCinemahall() {
            await removeCinemahallReleases(props.selectedReleases);
            props.releasesList.disableSelectionMode();
            props.releasesList.refreshCinemahall();
        }

        async function addToHided() {
            await addHidedReleases(props.selectedReleases);
            props.releasesList.disableSelectionMode();
        }

        async function removeFromHided() {
            await removeHidedReleases(props.selectedReleases);
            props.releasesList.disableSelectionMode();
        }

		return {
            addToHided,
            removeFromHided,
            addSelectedToFavorites,
            removeSelectedToFavorites,
            addSelectedToSeens,
            removeSelectedToSeens,
            addSelectedToCinemahall,
            removeSelectedToCinemahall
		};
	}
};