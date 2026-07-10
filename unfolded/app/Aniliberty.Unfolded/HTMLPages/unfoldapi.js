
// WebSocket hub
const webSocketHubObserver = {
    notification: function (type, message) {
    },
    user: function (message) {
    },
    synchronization: function (message) {
    }
};

const mainSocket = new WebSocket('ws://localhost:' + window.location.port + '/wshub/start');
mainSocket.onopen = (event) => {
    console.log('WebSocket successfully connected!');
};
mainSocket.onmessage = (event) => {
    const data = event.data;
    if (!data) return;

    const parts = data.split(':');
    if (parts.length < 2) return;

    const command = parts[0];
    const message = parts[1];

    switch (command) {
        case "ntc": // notification common
            webSocketHubObserver.notification("common", message);
            break;
        case "nte": // notification error
            webSocketHubObserver.notification("error", message);
            break;
        case "user": // events related to user account
            webSocketHubObserver.user(message);
            break;
        case "sync": // events related to synchronization
            webSocketHubObserver.synchronization(message);
            break;
    }
};
mainSocket.onclose = (event) => {
};
mainSocket.onerror = (error) => {
    console.error(error);
};

export function webSocketObserver() {
    return webSocketHubObserver;
}

export function sendWebSocketHubCommand(command, message) {
    if (mainSocket.readyState !== WebSocket.OPEN) return;

    mainSocket.send(command + ':' + message);
}

function paramsToQuery(params) {
    return new URLSearchParams(params).toString();
}

export async function getRelasesByFilter(model) {
    const response = await fetch(
        '/releases/list',
        {
            method: "POST",
            body: JSON.stringify(model),
            headers: {
                "Content-Type": "application/json"
            }
        }
    );
    return await response.json();
};
export async function getUserMarks(onlyForReleases) {
    let onlyForReleaseParameter = '';
    if (onlyForReleases) {
        onlyForReleaseParameter = onlyForReleases.map(a => `onlyForReleases=${a}`).join('&')
    }
    const response = await fetch('/releases/marks' + (onlyForReleaseParameter ? `?${onlyForReleaseParameter}` : ''));
    return await response.json();
};
export async function getReleaseEpisodes(releaseId) {
    const response = await fetch('/releases/episodes?releaseId=' + releaseId);
    return await response.json();
};
export async function getRelease(id) {
    const response = await fetch('/releases/release?id=' + id);
    return await response.json();
};
export async function getReleaseTorrents(id) {
    const response = await fetch('/releases/torrents?id=' + id);
    return await response.json();
};
export async function openMagnetInSystem(magnet) {
    await fetch('/releases/openmagnet?magnet=' + magnet);
};
export async function getReleaseFranchise(id) {
    const response = await fetch('/releases/franchise?id=' + id);
    return await response.json();
};


export async function synchronizeReleases() {
    await fetch('/sync/full');
};
export async function synchronizeUser() {
    await fetch('/sync/user');
};
export async function synchronizeFirstStart() {
    const response = await fetch('/sync/firststart');
    return response.json();
};
export async function synchronizeStatus() {
    const response = await fetch('/sync/status');
    return response.json();
};
export async function addFavorites(ids) {
    const array = ids.map(a => `ids=${a}`).join('&');
    await fetch('/sync/addfavorites?' + array);
};
export async function removeFavorites(ids) {
    const array = ids.map(a => `ids=${a}`).join('&');
    await fetch('/sync/removefavorites?' + array);
};

export async function authorizationByLoginPass(login, password) {
    const response = await fetch(
        '/auth/login?username=' + login,
        {
            method: "POST",
            body: JSON.stringify(password),
            headers: {
                "Content-Type": "application/json"
            }
        }
    );
    //if (!response.ok) throw new Error(`HTTP error, status = ${response.status}`);

    return await response.json();
};

export async function getPageSettings(page) {
    const response = await fetch('/settings/bypage?page=' + page);
    return await response.json();
};
export async function saveSettings(page, model) {
    await fetch('/settings/save/' + page, {
        method: "POST",
        body: JSON.stringify(model),
        headers: {
            "Content-Type": "application/json"
        }
    });
};

