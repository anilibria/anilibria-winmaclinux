import os
import subprocess
import shutil

def get_dependencies(executable_path):
    dependencies = []
    try:
        output = subprocess.check_output(['ldd', executable_path], universal_newlines=True)
        for line in output.splitlines():
            parts = line.split('=>')
            if len(parts) > 1:
                dependencies.append(parts[1].split('(')[0].strip())
    except subprocess.CalledProcessError as e:
        print(f"Error on ldd: {e}")
    return dependencies

def copy_dependencies(dependencies, target_dir):
    if not os.path.exists(target_dir):
        os.makedirs(target_dir)

    for dep in dependencies:
        try:
            shutil.copy(dep, target_dir)
            print(f"File copied: {dep} в {target_dir}")
        except Exception as e:
            print(f"Can't copy {dep}: {e}")

def main():
    executable_path = 'build/AniLiberty'
    target_dir = 'AppDir/usr/lib'

    dependencies = get_dependencies(executable_path)
    xcbDependencies = get_dependencies('AppDir/usr/lib/plugins/platforms/libqxcb.so')
    dependencies.extend(xcbDependencies)
    waylandDependencies = get_dependencies('AppDir/usr/lib/plugins/platforms/libqwayland-generic.so')
    dependencies.extend(waylandDependencies)
    waylandeglDependencies = get_dependencies('AppDir/usr/lib/plugins/platforms/libqwayland-egl.so')
    dependencies.extend(waylandeglDependencies)
    libqtquickcontrols2pluginDependencies = get_dependencies('AppDir/usr/lib/qml/QtQuick/Controls/libqtquickcontrols2plugin.so')
    dependencies.extend(libqtquickcontrols2pluginDependencies)
    libqtquickcontrols2materialstylepluginDependencies = get_dependencies('AppDir/usr/lib/qml/QtQuick/Controls/Material/libqtquickcontrols2materialstyleplugin.so')
    dependencies.extend(libqtquickcontrols2materialstylepluginDependencies)
    libqtquickcontrols2basicstylepluginDependencies = get_dependencies('AppDir/usr/lib/qml/QtQuick/Controls/Basic/libqtquickcontrols2basicstyleplugin.so')
    dependencies.extend(libqtquickcontrols2basicstylepluginDependencies)
    libparticlespluginDependencies = get_dependencies('AppDir/usr/lib/qml/QtQuick/Particles/libparticlesplugin.so')
    dependencies.extend(libparticlespluginDependencies)
    print(f"Founed dependencies: {dependencies}")
    
    copy_dependencies(dependencies, target_dir)

if __name__ == "__main__":
    main()
