# Перепаковка deb файла
В случае если у Вас возникает ошибка при установке пакета вида:
```bash
dpkg-deb: ошибка: в архиве «anilibria_X.X.X_amd64.deb» используется
неизвестное метод сжатия для «control.tar.zst», прекращаем.
```
Корень ошибки в том что пакеты упакованы архиватором zstd который не поддерживается в Вашей версии dpkg.

## Подготовка
Вначале необходимо проверить наличие установленных пакетов zstd, ar, tar.

## Как перепаковать пакет
```bash
ar x anilibria_X.X.X_amd64.deb
mkdir -p ./tmp/DEBIAN
zstd -d < control.tar.zst | xz > control.tar.xz
zstd -d < data.tar.zst | xz > data.tar.xz
tar -xf control.tar.xz -C ./tmp/DEBIAN
tar -xf data.tar.xz -C ./tmp/DEBIAN
mv debian-binary ./tmp
dpkg-deb -b tmp anilibria_X.X.X_amd64.deb
```
