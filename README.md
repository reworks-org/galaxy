# starlight v0.0.0

[![Windows Build status](https://ci.appveyor.com/api/projects/status/ac0ec6gtxl7776y5?svg=true)](https://ci.appveyor.com/project/reworks/starlight)
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
./build-macosx.sh
```

### Linux:
```
git clone --recursive --branch=master https://github.com/reworks/starlight.git reworks/starlight
./build-linux.sh
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