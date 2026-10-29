#!/usr/bin/env python
from glob import glob
from pathlib import Path
import os

# define tests
AddOption(
    "--tests",
    dest="tests",
    default="no",
    help="Enable tests (yes/no).",
)

# TODO: Do not copy environment after godot-cpp/test is updated <https://github.com/godotengine/godot-cpp/blob/master/test/SConstruct>.
env = SConscript("godot-cpp/SConstruct")

# Check homebrew path on macos
if env["platform"] == "macos":
    homebrew_path = '/opt/homebrew'
    if os.path.exists(homebrew_path):
        print(f"Homebrew detected at {homebrew_path}")
        env.Append(CPPPATH=[f'{homebrew_path}/include'])
        env.Append(LIBPATH=[f'{homebrew_path}/lib'])

        # add homebrew pkg-config path to env
        if 'PKG_CONFIG_PATH' in os.environ:
            env['ENV']['PKG_CONFIG_PATH'] = f"{homebrew_path}/lib/pkgconfig:{os.environ.get('PKG_CONFIG_PATH', '')}"

    if os.path.exists('/usr/local/lib'):
        env.Append(LIBPATH=['/usr/local/lib'])

    env.Append(LIBS=['zmq'])

# Check linuxbrew / vcpkg path on linux
if env["platform"] == "linux":
    linuxbrew_path = '/home/linuxbrew/.linuxbrew'
    if os.path.exists(linuxbrew_path):
        print(f"Linuxbrew detected at {linuxbrew_path}")
        env.Append(CPPPATH=[f'{linuxbrew_path}/include'])
        env.Append(LIBPATH=[f'{linuxbrew_path}/lib'])

        # add linuxbrew pkg-config path to env
        if 'PKG_CONFIG_PATH' in os.environ:
            env['ENV']['PKG_CONFIG_PATH'] = f"{linuxbrew_path}/lib/pkgconfig:{os.environ.get('PKG_CONFIG_PATH', '')}"

    vcpkg_linux_path = os.path.expanduser('~/vcpkg')
    if os.path.exists(vcpkg_linux_path):
        print(f"VCPKG detected at {vcpkg_linux_path}")
        env.Append(CPPPATH=[f'{vcpkg_linux_path}/installed/x64-linux/include'])
        env.Append(LIBPATH=[f'{vcpkg_linux_path}/installed/x64-linux/lib'])

        # add vcpkg pkg-config path to env
        if 'PKG_CONFIG_PATH' in os.environ:
            env['ENV']['PKG_CONFIG_PATH'] = f"{vcpkg_linux_path}/installed/x64-linux/share/pkgconfig:{os.environ.get('PKG_CONFIG_PATH', '')}"

    env.Append(LIBS=['zmq'])

# Check vcpkg path on windows
if env["platform"] == "windows":
    vcpkg_path = 'C:/vcpkg'
    if os.path.exists(vcpkg_path):
        print(f"VCPKG detected at {vcpkg_path}")
        vcpkg_lib_dir = f'{vcpkg_path}/installed/x64-windows/lib'
        env.Append(CPPPATH=[f'{vcpkg_path}/installed/x64-windows/include'])
        env.Append(LIBPATH=[vcpkg_lib_dir])

        # add vcpkg pkg-config path to env
        if 'PKG_CONFIG_PATH' in os.environ:
            env['ENV']['PKG_CONFIG_PATH'] = f"{vcpkg_path}/installed/x64-windows/share/pkgconfig:{os.environ.get('PKG_CONFIG_PATH', '')}"

        zmq_libs = glob(f'{vcpkg_lib_dir}/libzmq*.lib')
        if zmq_libs:
            env.Append(LIBS=[Path(zmq_libs[0]).stem])
        else:
            env.Append(LIBS=['libzmq'])
    else:
        env.Append(LIBS=['libzmq'])

if env["platform"] != "web":
    env.Append(CXXFLAGS=[f'-I{inc}' for inc in env['CPPPATH']])

# Add source files.
env.Append(CPPPATH=["src/"])
sources = Glob("src/*.cpp")

# # Add lz4 source files.
# env.Append(CPPPATH=["lz4/lib/"])
# sources += Glob("lz4/lib/*.c")

# Find gdextension path even if the directory or extension is renamed (e.g. project/addons/example/example.gdextension).
(extension_path,) = glob("project/addons/*/*.gdextension")

# Find the addon path (e.g. project/addons/example).
addon_path = Path(extension_path).parent

# Find the project name from the gdextension file (e.g. example).
project_name = Path(extension_path).stem

# TODO: Cache is disabled currently.
# scons_cache_path = os.environ.get("SCONS_CACHE")
# if scons_cache_path != None:
#     CacheDir(scons_cache_path)
#     print("Scons cache enabled... (path: '" + scons_cache_path + "')")

# Create the library target (e.g. libexample.linux.debug.x86_64.so).
debug_or_release = "release" if env["target"] == "template_release" else "debug"
if env["platform"] == "macos":
    library = env.SharedLibrary(
        "{0}/bin/lib{1}.{2}.{3}.framework/{1}.{2}.{3}".format(
            addon_path,
            project_name,
            env["platform"],
            debug_or_release,
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "{}/bin/lib{}.{}.{}.{}{}".format(
            addon_path,
            project_name,
            env["platform"],
            debug_or_release,
            env["arch"],
            env["SHLIBSUFFIX"],
        ),
        source=sources,
    )

# if tests=yes, TESTS_ENABLED is defined in the source code.
if GetOption("tests") == "yes":
    env.Append(CPPPATH=["test/"])
    sources = Glob("test/*.cpp")
    
    # run test_main
    test_main = env.Program("test/test_main", sources)
    env.Depends(test_main, library)
    env.AlwaysBuild(test_main)
    env.AddPostAction(test_main, "./test/test_main")

    Default([library, test_main])

else:
    Default(library)
