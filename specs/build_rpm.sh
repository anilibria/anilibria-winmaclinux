#!/usr/bin/bash -e

# setup source code
cd ~/anilibria-winmaclinux
git pull
sed -i 's/\/opt\/\$\$[{]TARGET[}]/\/usr\/local/g' AniLibria.pro
# remove lines in Anilibria.pro
# else: unix:!android: target.path
# !isEmpty(target.path) {
# ..
# }
# change line anilibria.desktop
# Exec=usr/bin/AniLibria

# refresh spec file
#cp ../spec/anilibria.spec ~/rpmbuild/SPECS

# create tarball
tar -czvf anilibria-winmaclinux-1.0.5.tar.gz .
mv anilibria-winmaclinux-1.0.5.tar.gz ~/rpmbuild/SOURCES

# build rpm
cd ~/rpmbuild/SPECS
rpmbuild -ba -vv anilibria.spec
