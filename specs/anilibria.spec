Name: anilibria-winmaclinux
Version: 1.0.3
License: GPLv3+
Release: 1%{?dist}
Summary: Application for watching anime from AniLibria website
URL: https://github.com/anilibria/%{name}
Source0: %{url}/releases/download/%{version}/%{name}-%{version}.tar.gz
 
BuildRequires: qt5-qtbase-devel
BuildRequires: qt5-qtwebview-devel
BuildRequires: qt5-qtwebengine-devel
BuildRequires: qt5-qtmultimedia-devel
BuildRequires: qt5-qtsvg-devel
BuildRequires: qt5-qtwebsockets-devel
BuildRequires: qt5-qtdeclarative-devel
BuildRequires: qt5-qtquickcontrols2-devel
BuildRequires: qt5-qtquickcontrols 
BuildRequires: libappstream-glib
BuildRequires: gstreamer1-libav
BuildRequires: gcc-c++
BuildRequires: gcc
 
%description
Application for watching anime from AniLibria website.

%global debug_package %{nil}

%prep
%setup -n src

%build
%qmake_qt5 -recursive     
%make_build
 
%install
%make_install

%files
%defattr(-,root,root, -)
