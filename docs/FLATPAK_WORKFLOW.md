# Flatpak workflow

## Коммит workflow без автоматического запуска

Файл GitHub Actions можно добавить в репозиторий и не запускать на ветке
`flatpak`. В текущей конфигурации workflow запускается только при push в
ветку `deploy`:

```yaml
on:
  push:
    branches: [deploy]
```

Поэтому push в `flatpak` workflow не запускает.

Добавить workflow в commit:

```bash
git add .github/workflows/flatpak.yml
git commit -m "Update Flatpak CI to KDE 6.11"
git push
```

## Ручной запуск

Если workflow вообще не должен запускаться автоматически, замените триггер на:

```yaml
on:
  workflow_dispatch:
```

После этого запуск выполняется вручную в GitHub Actions через **Run workflow**.

## Ограничение GitHub OAuth

GitHub может отклонить push, изменяющий файл `.github/workflows/*`, если
текущий OAuth-токен не имеет `workflow` scope:

```text
refusing to allow an OAuth App to create or update workflow
.github/workflows/flatpak.yml without workflow scope
```

Обновить авторизацию GitHub CLI:

```bash
gh auth refresh -h github.com -s workflow
gh auth status
git push
```

Альтернатива — использовать Personal Access Token с правом `workflow`.

## Проверка

Узнать текущую ветку и статус:

```bash
git branch --show-current
git status
```

Проверить, что workflow не запускается на `flatpak`:

```bash
git push origin flatpak
```

Workflow запустится только при push в `deploy`, если используется триггер
`on.push.branches: [deploy]`.
