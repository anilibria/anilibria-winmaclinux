#!/bin/bash

echo "Script for generate deb package"

cd ~/anilibria-winmaclinux
git pull
cd src
qmake
sudo make

echo "Application builded"

# create deb package

cd ~/anilibria-winmaclinux/deb

mkdir tmp
dpkg-deb -R pac.deb tmp

sed -i "/Version: 1.1.11/c\Version: $1" tmp/DEBIAN/control
sed -i "/Version=1.0/c\Version=$1" tmp/usr/share/applications/AniLibria.desktop

yes | cp /home/tester/anilibria-winmaclinux/src/AniLibria tmp/usr/bin/

dpkg-deb -b tmp anilibria_$1_amd64.deb

rm -rf tmp

echo "Package repacked"
