## Установка на Linux дистрибутивы отличные от NixOS с помощью пакетного менеджера Nix

1. Установка Nix

    Рекомендуется использовать [неофициальный установщик от DeterminateSystems](https://github.com/DeterminateSystems/nix-installer).

    ```sh
    curl --proto '=https' --tlsv1.2 -sSf -L https://install.determinate.systems/nix | sh -s -- install
    ```

    После завершения работы установщика надо перезапустить сессию и проверить что nix установился корректно

    ```sh
    nix profile install nixpkgs/nixos-unstable#hello
    hello
    ```

    Должна появится приветственная надпись на языке системы

2. Установка приложения AniLibria

    Выполнить команду

    ```nix
    nix profile install "github:anilibria/anilibria-winmaclinux#anilibria-winmaclinux.<gpu>"
    ```

    Где `<gpu>` заменить на:

    * nvidia - если видеокарта от nvidia и используется драйвер nvidia
    * nvidia-hybrid - если гибридная графика nvidia
    * amd - если видеокарта от amd
    * intel - если видеокарта от intel (встроенные в процессор)

    Автоматический выбор в зависимости от видеокарты (НЕ РЕКОМЕНДУЕТСЯ):

    ```nix
    nix profile install "github:anilibria/anilibria-winmaclinux#anilibria-winmaclinux.auto" --impure
    ```

3. Добавление приложения в меню приложений

    Чтобы приложение появилось в меню приложений нужно добавить директорию ~/.nix-profile/share в переменную окружения $XDG_DATA_DIRS.

    Для этого надо добавить строчку `export XDG_DATA_DIRS="$HOME/.nix-profile/share:$XDG_DATA_DIRS"` в файл `~/.profile`

    Сделать это можно вручную, через текстовый редактор или командой

    ```sh
    echo 'export XDG_DATA_DIRS="$HOME/.nix-profile/share:$XDG_DATA_DIRS"' >> ~/.profile
    ```

    Вероятно после этого надо будет перезапустить сессию

    Если после этого приложение не появилось в меню приложений, проконсультируйтесь в документации вашего дистрибутива или графической оболочки про выставление переменных окружения. Не смотря на отсутствие в меню, приложение должно быть доступно по команде `AniLibria` в терминале.

### Как найти приложение?

Приложение будет доступно из меню приложений и в терминале по имени AniLibria.

### Как обновить приложение?

Если при установке явно выбрали gpu:

```sh
nix profile upgrade '.*'
```

Если при установке выбрали auto:

```sh
nix profile upgrade '.*' --impure
```

После обновления проверьте что программа работает, если что-то не так, чтобы откатить обновление выполните

```sh
nix profile rollback
```

После успешного обновления чтобы удалить старые версии выполните команду

```sh
nix-collect-garbage -d
```
