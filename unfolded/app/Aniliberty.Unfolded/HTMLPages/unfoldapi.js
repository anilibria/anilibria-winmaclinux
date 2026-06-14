
function paramsToQuery(params) {
    return new URLSearchParams(params).toString();
}

export async function getRelasesPage(page) {
    const response = await fetch('/releases/list', paramsToQuery({ "page": page }));
    return await response.json();
};