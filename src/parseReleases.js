WorkerScript.onMessage = function(message) {
    const jsonData = JSON.parse(message.releasesJson);

    if (!jsonData.status) {
        //TODO: handle error situation
    }

    const jsonReleases = jsonData.data.items;

    WorkerScript.sendMessage({ releases: JSON.stringify(jsonReleases) })
}
