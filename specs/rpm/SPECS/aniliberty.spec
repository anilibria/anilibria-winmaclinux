Name:           aniliberty
Version:        2.2.35
Release:        1%{?dist}
Packager:	https://github.com/anilibria
Summary:        AniLiberty.Qt cross platform desktop client

License:        GPL-3.0
URL:            https://aniliberty.top
Source0:        %{name}-%{version}.tar.gz

Requires:       mpv-libs, qt6-qtbase, qt6-qtdeclarative, qt6-qtwebsockets, qt6-qtsvg
Requires:       libxkbcommon, qt6-qtquickcontrols2
BuildArch:      x86_64

%global debug_package %{nil}

%description
Desktop client for AniLiberty. Application allow watch online anime and synchronize with website favorites.

%prep
%setup -q

%build

%install
rm -rf %{buildroot}
cp -rp %{_builddir}/%{name}-%{version}/* %{buildroot}/

%files
%defattr(-,root,root,-)
/usr/local/bin/aniliberty
/usr/share/applications/aniliberty.desktop
/usr/share/icons/hicolor/16x16/apps/aniliberty.png
/usr/share/icons/hicolor/16x16@2/apps/aniliberty.png
/usr/share/icons/hicolor/32x32/apps/aniliberty.png
/usr/share/icons/hicolor/32x32@2/apps/aniliberty.png
/usr/share/icons/hicolor/128x128/apps/aniliberty.png
/usr/share/icons/hicolor/256x256/apps/aniliberty.png
/usr/share/icons/hicolor/256x256@2/apps/aniliberty.png
/usr/share/icons/hicolor/512x512/apps/aniliberty.png
/usr/share/icons/hicolor/512x512@2/apps/aniliberty.png
/usr/share/doc/aniliberty/changelog.gz
/usr/share/doc/aniliberty/copyright

%changelog
* %(date "+%a %b %d %Y") - 2.2.35
- Исправлен пункт "Синхронизация всех релизов" на странице Каталог релизов
- Добавлен пункт "Синхронизация постеров с заменой" на странице Каталог релизов
- Добавлен пункт "Синхронизация постеров только новые" на странице Каталог релизов
- Добавлен пункт "Создать шару из кеша" на странице Каталог релизов
- Добавлен пункт "Загрузить шару кеша" на странице Каталог релизов
