 export const sortingFields = [
    {
        title: "Обновлению",
        id: 0
    },
    {
        title: "Расписанию",
        id: 1
    },
    {
        title: "Имени",
        id: 2
    },
    {
        title: "Году",
        id: 3
    },
    {
        title: "Рейтингу",
        id: 4
    },
    {
        title: "Статусу",
        id: 5
    },
    {
        title: "Ориг. имени",
        id: 6
    },
    {
        title: "Истории открытия",
        id: 7
    },
    {
        title: "Истории просмотра",
        id: 8
    },
    {
        title: "Сезону",
        id: 9
    },
    {
        title: "Избранности",
        id: 10
    },
    {
        title: "Просмотренности",
        id: 11
    },
    {
        title: "Части серии",
        id: 12
    },
    {
        title: "Истории",
        id: 13
    },
];

export const sections = [
    {
        title: "Все релизы",
        id: 0
    },
    {
        title: "Избранное",
        id: 1
    },
    {
        title: "Уведомления",
        id: 2
    },
    {
        title: "Расписание",
        id: 3
    },
    {
        title: "Алфавит",
        id: 4
    },
    {
        title: "История",
        id: 6
    },
    {
        title: "Группы",
        id: 5
    },
    {
        title: "Просмотр",
        id: 7
    },
    {
        title: "Подборки",
        id: 8
    }
];

export const historySections = [
    {
        title: "Не выбрано",
        id: -1
    },
    {
        title: "История",
        id: 40
    },
    {
        title: "История просмотра",
        id: 41
    }
];

export const seenMarkSections = [
    {
        title: "Не выбрано",
        id: -1
    },
    {
        title: "Просмотренные",
        id: 42
    },
    {
        title: "Просматриваемые",
        id: 43
    },
    {
        title: "Не просмотренные",
        id: 44
    },
    {
        title: "Скрытые релизы",
        id: 45
    },
    {
        title: "Просмотренные до конца",
        id: 46
    },
    {
        title: "Просмотренные не до конца",
        id: 47
    }
];

export const setsSections = [
    {
        title: "Не выбрано",
        id: -1
    },
    {
        title: "Фильмы",
        id: 48
    },
    {
        title: "Самые популярные",
        id: 49
    },
    {
        title: "Часть серии",
        id: 50
    },
    {
        title: "Завершенные",
        id: 51
    },
    {
        title: "Популярное в текущем году",
        id: 52
    },
    {
        title: "В кинозале",
        id: 53
    },
    {
        title: "Текущий сезон",
        id: 54
    },
    {
        title: "Не текущий сезон",
        id: 55
    }
];

export function getPublishDay(value) {
    switch (value) {
        case 1: return 'пн';
        case 2: return 'вт';
        case 3: return 'ср';
        case 4: return 'чт';
        case 5: return 'пт';
        case 6: return 'сб';
        case 7: return 'вс';
        default: return '';
    }
}

export const openLinkMode = [
    {
        title: "В текущем окне",
        id: 0
    },
    {
        title: "В новом окне",
        id: 1
    }
];

export const notificationModes = [
    {
        title: "Выключены",
        id: 0
    },
    {
        title: "Уведомления по всем релизам",
        id: 1
    },
    {
        title: "Уведомления только по избранному",
        id: 2
    },
];