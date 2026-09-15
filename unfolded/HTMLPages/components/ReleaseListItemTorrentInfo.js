import { downloadTorrent, openInExternalTorrentClient } from '/static/unfoldapi.js'

export default {
	props: ['release', 'hasactivetorrent', 'useInternalTorrectClient', 'torrentcount', 'torrentdownloaded', 'openLinkMode', 'videomode'],
	template: `
<img
    v-if="release.countTorrents > 0 && !hasactivetorrent && useInternalTorrectClient"
    class="clickable-image"
    src="/static/icons/mainmenu/torrent.svg"
    width="20"
    height="20"
    loading="lazy"
    title="Скачать релиз через встроенный торрент клиент"
    @click.stop="downloadInTorrent(release.id)"
/>
<img
    v-if="release.countTorrents > 0 && !hasactivetorrent && !useInternalTorrectClient"
    src="/static/icons/mainmenu/torrent.svg"
    width="20"
    height="20"
    loading="lazy"
/>
<span v-if="release.countTorrents > 0 && !hasactivetorrent" v-for="(torrentCodec, codecIndex) in release.torrentCodecs" :key="codecIndex">
    <a
        class="default-link"
        href=""
        @click.prevent.stop="downloadInExternalClient(release.id, torrentCodec)">
        {{ clearCodec(torrentCodec.name) }}
    </a>
    <span>{{ codecIndex < release.torrentCodecs.length - 1 ? ' / ' : '' }}</span>
</span>
<div class="flex flex-row" v-if="hasactivetorrent">
    <img
        class="clickable-image"
        src="/static/icons/mainmenu/torrent.svg"
        width="20"
        height="20"
        loading="lazy"
        :title="torrentcount === torrentdownloaded ? 'Открыть скаченный релиз в видеоплеере' : 'Релиз скачивается...'"
        @click.stop="openTorrentInVideoplayer(release.id, torrentcount, torrentdownloaded)"
    />
    &nbsp;
    <span>{{ torrentdownloaded }} из {{ torrentcount }}</span>
</div>`,
	setup(props) {

        function clearCodec(codec) {
            let res = codec.toLowerCase();
            const index = res.indexOf('/');
            if (index > -1) return res.substring(index + 1);

            return res;
        }

        function openTorrentInVideoplayer(releaseId, torrentcount, torrentdownloaded) {
            if (torrentcount !== torrentdownloaded) return;

            const mode = props.videomode ? `mode=${props.videomode}&` : '';

            if (props.openLinkMode) {
                window.open(`videoplayer.html?${mode}id=` + releaseId, '_blank');
            } else {
                window.location.href = `videoplayer.html?${mode}id=` + releaseId;
            }
        }

        async function downloadInExternalClient(id, torrentCodec) {
            await openInExternalTorrentClient(id, torrentCodec.hash);
        }

        async function downloadInTorrent(id) {
            await downloadTorrent(id);
        }


		return {
			clearCodec,
            openTorrentInVideoplayer,
            downloadInExternalClient,
            downloadInTorrent
		};
	}
};