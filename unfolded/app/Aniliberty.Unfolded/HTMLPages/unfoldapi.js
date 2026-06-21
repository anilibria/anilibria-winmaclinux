
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

export async function synchronizeReleases() {
    await fetch('/sync/full');
};

export async function synchronizeUser() {
    await fetch('/sync/user');
};