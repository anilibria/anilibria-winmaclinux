
let mainMenu = [];
let userData = {};

async function getMainMenuInternal() {
    const response = await fetch('/mainmenu/items');
    return await response.json();
}

async function getUserDataInternal() {
    const response = await fetch('/mainmenu/user');
    return await response.json();
}


mainMenu = await getMainMenuInternal();
userData = await getUserDataInternal();

export function getMainMenu() {
    return mainMenu;
};

export function getUserData() {
    return userData;
};

export async function refreshMainMenu() {
    mainMenu = await getMainMenuInternal();
    userData = await getUserDataInternal();
};