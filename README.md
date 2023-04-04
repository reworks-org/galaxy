# galaxy
[![Windows](https://ci.appveyor.com/api/projects/status/ww31j6b22u7bo1ua?svg=true)](https://ci.appveyor.com/project/reworks/galaxy)
[![Documentation Status](https://travis-ci.com/DomRe/galaxy.svg?branch=master)](https://domre.github.io/galaxy/)
[![Code Quality](https://app.codacy.com/project/badge/Grade/1cac439022e2417fa82b5dbc2c320030)](https://www.codacy.com/gh/DomRe/galaxy/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=DomRe/galaxy&amp;utm_campaign=Badge_Grade)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/4377/badge)](https://bestpractices.coreinfrastructure.org/projects/4377)
[![Apache 2.0 Licensed](https://img.shields.io/badge/license-apache-blue.svg)](./LICENSE.txt)

Real-Time C++20 Game/App Engine. Built on data-driven design principles and agile software engineering. 

![galaxy](logo.png?raw=true "galaxy")

## Building

Note that only 64bit is supported.

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
git submodule update --init --recursive
cd galaxy
Open galaxy.sln and go Build -> Rebuild Solution.
```

### Linux:
*Only Ubuntu is supported.*

[CMake v3.24](https://cmake.org/download/)

[Ubuntu 22.04 LTS](https://ubuntu.com/download/desktop)

-- OR --

[WSL2 - Ubuntu](https://docs.microsoft.com/en-us/windows/wsl/install)

```
Coming Soon.
```

### MacOS:
```
No.
```

### Scripts:
There are additional helper scripts located in the [scripts folder](https://github.com/DomRe/galaxy/tree/master/scripts).

In order to run these, you require a ```MSYS2 installation``` installed on your ```PATH```.

## Documentation
See [online documentation](https://domre.github.io/galaxy/).

## Features
```
todo
```

## Contributing / Feedback / Issues
Feel free to open an issue for feedback, bug reports, ehancements, issues, etc,
and pull requests for contributions. (Don't forget to add yourself to the credits section below.)

### Credits
* Uses SMAA. Copyright (C) 2011 by Jorge Jimenez, Jose I. Echevarria,  Belen Masia, Fernando Navarro and Diego Gutierrez.
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
* Resources used for testing:
	* Various from [OpenGameArt](https://opengameart.org/).
* https://github.com/ocornut/imgui/issues/707
* https://materialdesignicons.com/