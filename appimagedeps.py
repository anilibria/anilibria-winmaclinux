import os
import subprocess
import shutil

def get_dependencies(executable_path):
    dependencies = []
    try:
        output = subprocess.check_output(['ldd', executable_path], universal_newlines=True)
        for line in output.splitlines():
            print(f"Line founded: {line}")
            parts = line.split()
            if len(parts) > 0 and '=> ' in line:
                dependencies.append(parts[0])
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
    print(f"Founed dependencies: {dependencies}")
    
    copy_dependencies(dependencies, target_dir)

if __name__ == "__main__":
    main()