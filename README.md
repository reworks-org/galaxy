# galaxy
[![Windows](https://ci.appveyor.com/api/projects/status/ww31j6b22u7bo1ua?svg=true)](https://ci.appveyor.com/project/reworks/galaxy)
[![Linux](https://github.com/DomRe/galaxy/actions/workflows/linux.yml/badge.svg)](https://github.com/DomRe/galaxy/actions/workflows/linux.yml)
[![Documentation Status](https://travis-ci.com/DomRe/galaxy.svg?branch=master)](https://domre.github.io/galaxy/)
[![Code Quality](https://api.codacy.com/project/badge/Grade/1cac439022e2417fa82b5dbc2c320030)](https://www.codacy.com/manual/DomRe/galaxy?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=DomRe/galaxy&amp;utm_campaign=Badge_Grade)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/4377/badge)](https://bestpractices.coreinfrastructure.org/projects/4377)
[![Apache 2.0 Licensed](https://img.shields.io/badge/license-apache-blue.svg)](./LICENSE.txt)

Real-Time C++20 Game/App Engine. Built on data-driven design principles and agile software engineering. 

![galaxy](logo.png?raw=true "galaxy")

## Building

Requirements:
[64bit OS](https://en.wikipedia.org/wiki/X86-64)
[Doxygen](https://www.doxygen.nl/index.html)
[Doxygen Dark Theme](https://github.com/MaJerle/doxygen-dark-theme)

### Windows:
[Visual Studio 2022](https://visualstudio.microsoft.com/vs/)

Major Components:
```
Desktop development with C++
Game development with C++
```
Minor Components:
```
Windows Universal CRT SDK
Windows SDK 10.0.20348.0
Windows Universal C Runtime
```
Building Instructions:
```
git clone --recursive https://github.com/DomRe/galaxy.git galaxy
cd galaxy
Open galaxy.sln and go Build -> Rebuild Solution.
```

### Linux:
*Only Ubuntu is supported.*

[CMake v3.22](https://cmake.org/download/)
[Ubuntu 22.04](https://ubuntu.com/download/desktop)
-- OR --
[WSL2 - Ubuntu (22.04)](https://docs.microsoft.com/en-us/windows/wsl/install)
[VSCode](https://code.visualstudio.com/)
If developing on WSL2, you need this:
[Remote - WSL](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-wsl)
Other required extensions will be automatically recommended to you.

Required Packages:
```git gh build-essential g++ gcc g++-12 gcc-12 gdb make cmake ninja-build wget ca-certificates rsync zip libfreetype-dev bzip2 software-properties-common xorg-dev libz-dev```
If using WSL2, I recommend installing all packages from [this microsoft guide](https://docs.microsoft.com/en-us/windows/wsl/tutorials/gui-apps) in order to get the basic packages required for video, audio, etc.
*Chrome, Teams and Edge not required, and you can uninstall the apps immediately afterwards, just leave their required packages installed.*

Building Instructions:
```
sudo apt update
sudo apt upgrade -y
Install above required packages.
git clone --recursive https://github.com/DomRe/galaxy.git galaxy
cd galaxy
Open "galaxy.code-workspace" in VSCode (or Remote - WSL).
Install the recommended extensions and restart.
Set the CMake kit is set to "GCC 12.0.1 x86_64-linux-gnu". You can set this from the bottom bar in VSCode, next to CMake. It looks like a spanner and wrench icon.
```

### Scripts:
There are additional helper scripts located in the [scripts folder](https://github.com/DomRe/galaxy/tree/master/scripts).
In order to run these on Windows, you require a ```MSYS2 installation``` installed on your ```PATH```.

## Documentation
See [online documentation](https://domre.github.io/galaxy/).

## Features
*May be out of date.*

- Uses Modern C++ design principles, built on C++20.
- Unit Testing and Sandbox Testing.
- Application and State Management.
- AI Algorithms.
- Async programming.
- Audio streaming and abstraction.
- Entity-Component-System.
- Config Engine.
- GUID generator.
- Scene Stack.
- Error Handling.
- Logging using Sinks and leveraging ```constexpr``` and ```if constexpr```.
- Event dispatcher based on subscribing and listeners.
- Virtual FileSystem
- Database Support using [SQLite](https://www.sqlite.org/index.html).
- Serialization to JSON.
- Real-Time 2D rendering framework.
	- Modern Core OpenGL 4.5
	- Animations
	- Text
	- Post-Processing
	- Primitives
	- Shading/Textures
	- Etc.
- Input Abstractions.
- Tiled Map parsing and rendering.
	- Supported:
		- Tiled 1.4.3 .json maps.
		- Loading maps from Tiled Worlds.
		- Base64 encoding.
		- zlib compression.
		- gzip compression.
	- Limitations:
		- Only renders right-down maps.
		- Only renders topdown objects.
	- Unsupported:
		- Tiled projects.
		- External Tiled assets.
		- Text objects.
		- Terrain.
		- Wang-Sets.
		- Tile Flipping.
- Compression/Decompression Algorithms.
- Rect Packing.
- Physics Engine using [Box2D](https://github.com/erincatto/box2d).
- Platform Abstractions.
- Resource Management.
	- Fonts
	- Languages
	- Audio
	- Scripts
	- Shaders
	- Textures
- [Lua](https://www.lua.org/) Scripting.
- [RMLUI](https://github.com/mikke89/RmlUi) Support.
- [ImGui](https://github.com/ocornut/imgui) Support.

## Contributing / Feedback / Issues
Feel free to open an issue for feedback, bug reports, ehancements, issues, etc,
and pull requests for contributions. (Don't forget to add yourself to the credits section below.)

### Credits
* Uses SMAA. Copyright (C) 2011 by Jorge Jimenez, Jose I. Echevarria,  Belen Masia, Fernando Navarro and Diego Gutierrez.
* [doxygen-dark-theme](https://github.com/MaJerle/doxygen-dark-theme).
* [sercode.ttf](http://www.dafont.com/secret-code.font).
	* Used in various icons and logos.
* [galaxy image](https://www.nasa.gov/image-feature/goddard/2020/hubble-probes-colorful-galaxy).
	* Image Credit: ESA/Hubble & NASA, A. Filippenko.
	* See [NASA media guidelines](https://www.nasa.gov/multimedia/guidelines/index.html).
	* Used for non-commerical purposes.
* [Supercluster Icon and Logo](https://images.nasa.gov/details-PIA17241).
    * Image Credit: NASA/JPL-Caltech/University of Wisconsin
    * See [NASA media guidelines](https://www.nasa.gov/multimedia/guidelines/index.html).
	* Used for non-commerical purposes.
* Any "dependency" i.e. "software library" should/will usually have an associated LICENSE/COPYING/AUTHORS file included.
* Resources used in sandbox:
	* Various from [OpenGameArt](https://opengameart.org/). No copyright infringement intended.
* https://learnopengl.com/
* https://github.com/ocornut/imgui/issues/707
* https://github.com/Asmodean-/SMAA-OpenGL/
* https://github.com/lettier/3d-game-shaders-for-beginners