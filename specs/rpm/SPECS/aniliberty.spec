Name:           aniliberty
Version:        2.2.34
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
* %(date "+%a %b %d %Y") - 2.2.34
- На странице "Приложения" исправлено удаление приложения если файлы и/или папка были удалены в файловой системе
- В приложение интегрировано приложение LocalCacheChecker как библиотека
- В меню кнопки синхронизации добавлены пункты "Синхронизация Типов", "Синхронизация последних изменений", "Синхронизация последних релизов", "Синхронизация всех релизов"


