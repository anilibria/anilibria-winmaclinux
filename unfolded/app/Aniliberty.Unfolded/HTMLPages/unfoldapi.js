
function paramsToQuery(params) {
    return new URLSearchParams(params).toString();
}

export async function getRelasesPage(model) {
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