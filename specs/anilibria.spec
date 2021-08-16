Name: anilibria-winmaclinux
Version: 1.0.3
License: GPLv3+
Release: 1%{?dist}
Summary: Application for watching anime from AniLibria website
URL: https://github.com/anilibria/%{name}
Source0: %{url}/archive/refs/tags/%{version}.tar.gz
 
BuildRequires: qt5-qtbase-devel
BuildRequires: qt5-qtwebview-devel
BuildRequires: qt5-qtmultimedia-devel
BuildRequires: qt5-qtsvg-devel
BuildRequires: qt5-qtwebsockets-devel
BuildRequires: qt5-qtdeclarative-devel
BuildRequires: qt5-qtquickcontrols2-devel
BuildRequires: qt5-qtquickcontrols 
BuildRequires: gstreamer1-libav
BuildRequires: libappstream-glib
BuildRequires: gcc-c++
BuildRequires: gcc
 
%description
Application for watching anime from AniLibria website.
 
%build
mkdir -p %{_target_platform}
pushd %{_target_platform}
	%qmake_qt5 \
	PREFIX=%{_prefix} \
	CONFIG+=anilibria-winmaclinux \
	..
popd
%make_build -C %{_target_platform}
 
%install
%make_install
 
%doc README.md
%license LICENSE
%{_bindir}/AniLibria
%{_datadir}/applications/anilibria.desktop
 
%changelog
* Mon Aug 16 2021 RaS <x6doj2tubzrf@mail.ru> - 1.0.3
- Initial SPEC release.