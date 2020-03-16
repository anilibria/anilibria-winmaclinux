.import "./QuickPromise/promise.js" as PromiseLibrary

const WebSiteUrl = "https://www.anilibria.tv";
const ApiIndexUrl = WebSiteUrl + "/public/api/index.php";

function executePostRequest(url, form, callback, failCallback) {
    return PromiseLibrary.promise(
        (resolve, reject) => {
            const request = new XMLHttpRequest();
            request.open("POST", url);
            request.onreadystatechange = function() {
                if (request.readyState !== XMLHttpRequest.DONE) {
                    reject({ status: -1, message: "" });
                } else {
                    if (request.status !== 200) {
                        reject({ status: request.status, message: request.statusText });
                        return;
                    }

                    resolve(JSON.parse(request.responseText));
                }
            }
            request.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
            request.send(Object.keys(form).map(a => a + "=" + form[a]).join("&"));
        }
    );
}

function getReleases(page, perPage) {
    return executePostRequest(ApiIndexUrl, { query: "list", page: page.toString(), perPage: perPage.toString() });
}


