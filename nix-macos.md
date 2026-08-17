## Установка на MacOS (arm) с помощью пакетного менеджера Nix

1. Установка Nix

    Рекомендуется использовать [официальный установщик Nix](https://nixos.org/download/#nix-install-macos).

    ```sh
    echo "experimental-features = nix-command flakes" > /tmp/nix.conf
    curl --proto '=https' --tlsv1.2 -L https://nixos.org/nix/install | sh --nix-extra-conf-file /tmp/nix.conf
    rm -f /tmp/nix.conf
    ```

2. Проверка работы Nix

    После завершения работы установщика надо перезапустить сессию и проверить что nix установился корректно

    ```sh
    nix run "github:NixOS/nixpkgs/nixos-unstable#hello"
    ```

    Должна появится приветственная надпись на языке системы

3. Установка приложения AniLibria

    Выполнить команду

    ```sh
    nix profile install "github:NixOS/nixpkgs/nixos-unstable#anilibria-winmaclinux"
    ```

### Как найти приложение?

Приложение будет доступно из меню приложений и в терминале по имени AniLibria или AniLiberty.

### Как обновить приложение?

```sh
nix profile upgrade --regex '.*anilibria-winmaclinux.*'
```

После обновления проверьте что программа работает, если что-то не так, чтобы откатить обновление выполните

```sh
nix profile rollback
```

После успешного обновления чтобы удалить все старые версии выполните команду

```sh
nix-collect-garbage -d
```
