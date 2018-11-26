# starlight

[![Build Status](https://ci.appveyor.com/api/projects/status/ac0ec6gtxl7776y5?svg=true)](https://ci.appveyor.com/project/reworks/starlight)
[![Docs Status](https://readthedocs.org/projects/starlight/badge/?version=latest)](https://starlight.readthedocs.io/en/latest/?badge=latest)
[![MIT Licensed](https://img.shields.io/badge/license-apache-blue.svg)](./LICENSE.txt)

Game programming library using Allegro 5.2.4, Lua 5.3, and C++17. This was built as a hobby for hobby projects.

## Building
You need git as a prereq to build. Other libraries are downloaded by the build script for you.

For Windows, you can use premake, but a solution file is provided.

### Windows:
```
Download repository using git or the web interface. Enter the directory.
Open starlight.sln
Select build configuration and architecture.
Build whole solution.
```

or

```
Download repository using git or the web interface. Enter the directory.
Run build.bat. This will use premake5 to generate build files for you.
Open starlight.sln
Select build configuration and architecture.
Build whole solution.
```


### Linux:
Please note that only Ubuntu 18.04 is tested and supported for building. 
You may need to add allegro 5.2 repositories to previous versions of Ubuntu to have the build script work. I.e.

```
sudo add-apt-repository ppa:allegro/5.2
sudo apt-get update
```

```
git clone --recursive --branch=master https://github.com/reworks/starlight.git build
cd build
chmod +x build.sh
sudo ./build.sh
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


## Documentation
Can be built from [doxygen](https://github.com/reworks/starlight/tree/master/docs) Doxyfile or viewed at [readthedocs](https://starlight.readthedocs.io/en/latest/).


## Features
- Uses Modern C++, including C++17 features.
- Uses [ennt](https://github.com/skypjack/entt) and built around ECS principles.
- Build system provided by [premake5](https://premake.github.io/download.html).
- Fast logging provided by [spdlog](https://github.com/gabime/spdlog).
- [ImGui](https://github.com/ocornut/imgui) for making and using debugging and utility UI.
- [sol2](https://github.com/ThePhD/sol2) for [Lua](https://www.lua.org/) integration.
- [libxml2](http://xmlsoft.org/downloads.html) for XML support.
- Built on top of [Allegro5](https://liballeg.org/).
- [TMX parser](https://github.com/baylej/tmx) for loading TMX maps.
- Linux support!

## Credits

All libraries used by the engine have a LICENSE.txt with them. This file includes licenses for libraries not included directly in this repo.
This also has credits for assets used in logos / sandbox testing.

sercode.ttf:
	Link: http://www.dafont.com/secret-code.font
	License: MIT


Resources used in sandbox demo:
Various from opengameart.org. No copyright infringement intended.

![starlight](logo.png?raw=true "starlight")