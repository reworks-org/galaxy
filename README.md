# galaxy
[![Windows](https://ci.appveyor.com/api/projects/status/ww31j6b22u7bo1ua?svg=true)](https://ci.appveyor.com/project/reworks/galaxy)
[![Documentation Status](https://travis-ci.com/DomRe/galaxy.svg?branch=master)](https://domre.github.io/galaxy/)
[![Code Quality](https://api.codacy.com/project/badge/Grade/1cac439022e2417fa82b5dbc2c320030)](https://www.codacy.com/manual/DomRe/galaxy?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=DomRe/galaxy&amp;utm_campaign=Badge_Grade)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/4377/badge)](https://bestpractices.coreinfrastructure.org/projects/4377)
[![Apache 2.0 Licensed](https://img.shields.io/badge/license-apache-blue.svg)](./LICENSE.txt)

Real-time full featured Game/App/Software Engine using C++20, featuring data-driven design and test-driven development. From games, to data visualisation, to beyond!

![galaxy](logo.png?raw=true "galaxy")

## Building

Hard Requirements:
64-bit OS.
[CMake](https://cmake.org/) 3.22

Soft Requirements:
[Doxygen](https://www.doxygen.nl/index.html) 1.9.2 and [Doxygen Dark Theme](https://github.com/MaJerle/doxygen-dark-theme) - Only if you want to build locally. Clone the dark theme into a folder named ```ddt``` in the root of the repo (i.e. next to Doxyfile).

### Windows:
You will need to install at least ```Visual Studio 2022 17.0```, and install the following:

Major Components:
```
Desktop development with C++
Game development with C++
Linux development with C++
```
Minor Components:
```
C++ CMake tools for Windows
C++ CMake tools for Linux
Windows Universal CRT SDK
Windows SDK 10.0.20348.0
Windows Universal C Runtime
```
Building Instructions:
```
git clone --recursive https://github.com/DomRe/galaxy.git galaxy_engine
git submodule update --init --recursive
cd galaxy_engine
Open galaxy.sln and go Build -> Rebuild Solution.
```

### Linux:
```
Coming Soon.
```

### MacOS:
```
Coming Soon.
```

## Documentation
See [online documentation](https://domre.github.io/galaxy/).

## Features
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
* Any "dependency" i.e. "software library" should/will usually have an associated LICENSE.txt/COPYING.txt file included.
* Resources used in sandbox:
	* Various from [OpenGameArt](https://opengameart.org/). No copyright infringement intended.
* https://learnopengl.com/
* https://skypjack.github.io/2019-02-14-ecs-baf-part-1/
* https://skypjack.github.io/2019-03-21-ecs-baf-part-2-insights/
* https://skypjack.github.io/2019-06-25-ecs-baf-part-4/
* https://skypjack.github.io/2019-09-25-ecs-baf-part-5/
* https://indiegamedev.net/2020/02/25/the-complete-guide-to-openal-with-c-part-2-streaming-audio/
* https://indiegamedev.net/2020/01/16/how-to-stream-ogg-files-with-openal-in-c/
* https://gist.github.com/tilkinsc/f91d2a74cff62cc3760a7c9291290b29
* https://github.com/ocornut/imgui/issues/707
* https://github.com/SSBMTonberry/tileson/
* https://github.com/Asmodean-/SMAA-OpenGL/
* https://github.com/lettier/3d-game-shaders-for-beginners