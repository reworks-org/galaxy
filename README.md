# starlight v0.0.0

[![Windows build status](https://ci.appveyor.com/api/projects/status/ac0ec6gtxl7776y5?svg=true)](https://ci.appveyor.com/project/reworks/starlight)
[![Linux & Mac OSX build status](https://travis-ci.org/reworks/starlight.svg?branch=master)](https://travis-ci.org/reworks/starlight)
[![Docs Status](https://readthedocs.org/projects/starlight/badge/?version=latest)](https://starlight.readthedocs.io/en/latest/?badge=latest)
[![MIT Licensed](https://img.shields.io/badge/license-apache-blue.svg)](./LICENSE.md)

Game programming library using Allegro 5.2.4, Lua 5.3, and C++17.

## Building
You need git as a prereq to build. Other libraries are downloaded by the build script for you.

On Mac OS X you also need [Homebrew](https://brew.sh/) installed.

### Windows:



### MacOSX:
```
git clone --recursive --branch=master https://github.com/reworks/starlight.git reworks/starlight
cd reworks/starlight
chmod +x build-macosx.sh
./build-macosx.sh
```

Then you can check the makefile for your platform configs and run make on it. E.g.:

```
make config=debug_macos all
make config=release_macos all
```


### Linux:
Please note that only Ubuntu 18.04 is tested and supported for building. Does build in a docker image.

```
git clone --recursive --branch=master https://github.com/reworks/starlight.git reworks/starlight
cd reworks/starlight
chmod +x build-linux.sh
sudo ./build-linux.sh
```

Then you can check the makefile for your platform configs and run make on it. E.g.:

```
make config=debug_linux32 all
make config=release_linux32 all
```

or

```
make config=debug_linux64 all
make config=release_linux64 all
```


## Docs
Can be built from [doxygen](https://github.com/reworks/starlight/tree/master/docs) Doxyfile or viewed at [readthedocs](https://starlight.readthedocs.io/en/latest/).


## Features
- C++17 codebase.
- Build around ECS principles.
- premake5 build system.
- Fast logging provided by loguru.
- Imgui for debug ui tools.
- Sol2 for lua integration.
- libxml2 for XML support.
- Allegro5 backend.
- tmx-c for loading and using tmx maps.
- Cross-Platform compatibility.

![starlight](logo.png?raw=true "starlight")