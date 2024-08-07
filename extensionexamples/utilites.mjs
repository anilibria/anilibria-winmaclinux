/*
    Anilibria.Qt utilites helper functions
    branch 1.X
*/

export const application = {
    getAppVersion() {
        return appVersion;
    },
    checkIfAppVersionIs(version) {
        return appVersion === version;
    }    
}

export const logger = {
    message(message) {
        return appExtension.log(message);
    }  
}

export const globalValues = {
    saveValue(name, value) {
        appExtension.saveValue(name,value);
    },
    readValue(name) {
        return appExtension.readValue(name);
    },
    deleteValue(name) {
        appExtension.deleteValue(name);
    }
}

function emptyFunc() {
}

export const httpClient = {
    get: function (url, options) {
        if (!url) {
            logger.message(`Parameter 'url' not specified!`);
            return;
        }
        const headers = options && options.headers ? options.headers : [];
        const callback = options && options.callback ? options.callback : emptyFunc;
        appExtension.makeHttpGet(url, headers, callback);
    },
    post: function (url, body, options) {
        if (!body) {
            logger.message(`Parameter 'body' not specified!`);
            return;
        }
        if (!url) {
            logger.message(`Parameter 'url' not specified!`);
            return;
        }
        const headers = options && options.headers ? options.headers : [];
        const callback = options && options.callback ? options.callback : emptyFunc;

        appExtension.makeHttpPost(url, headers, body, callback);
    }
};
