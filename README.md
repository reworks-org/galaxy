# galaxy
[![Build Status](https://ci.appveyor.com/api/projects/status/ac0ec6gtxl7776y5?svg=true)](https://ci.appveyor.com/project/reworks/galaxy)
[![Documentation Status](https://travis-ci.org/DomRe/galaxy.svg?branch=master)](https://domre.github.io/galaxy/)
[![Test Coverage](https://codecov.io/gh/DomRe/galaxy/branch/master/graph/badge.svg)](https://codecov.io/gh/DomRe/galaxy)
[![Code Quality](https://api.codacy.com/project/badge/Grade/1cac439022e2417fa82b5dbc2c320030)](https://www.codacy.com/manual/DomRe/galaxy?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=DomRe/galaxy&amp;utm_campaign=Badge_Grade)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/3923/badge)](https://bestpractices.coreinfrastructure.org/projects/3923)
[![Apache 2.0 Licensed](https://img.shields.io/badge/license-apache-blue.svg)](./LICENSE.txt)

Game Programming Library using C++17, OpenGL 4.5, Lua and JSON.

![galaxy](logo.png?raw=true "galaxy")

## Building
### Windows:
```
git clone --recursive https://github.com/DomRe/galaxy.git
msbuild galaxy.sln
```

### Unix:
```
// todo
```

If your submodules did not clone properly run:
```
git submodule update --init --recursive
```

## Documentation
See [online documentation](https://domre.github.io/galaxy/).

Or generate offline with Doxygen.

## Features
- Uses Modern C++ design principles, built on C++17.

```cpp
// todo
```

## Contributing / Feedback / Issues
Feel free to open an issue for feedback, bug reports, ehancements, issues, etc,
and pull requests for contributions. (Don't forget to add yourself to the credits section below.)

### Credits
* [doxygen-dark-theme](https://github.com/MaJerle/doxygen-dark-theme).
* [sercode.ttf](http://www.dafont.com/secret-code.font).
	* Used in logo.png & logo_docs.png.
* [galaxy image](https://www.nasa.gov/image-feature/goddard/2020/hubble-probes-colorful-galaxy).
	* Image Credit: ESA/Hubble & NASA, A. Filippenko.
	* See [NASA media guidelines](https://www.nasa.gov/multimedia/guidelines/index.html).
	* Used for non-commerical purposes.
* Any "dependency" i.e. "software library" will have an associated LICENSE.txt/COPYING.txt file included.
* Resources used in sandbox:
	* Various from [OpenGameArt](https://opengameart.org/). No copyright infringement intended.
* https://learnopengl.com/
* https://skypjack.github.io/2019-02-14-ecs-baf-part-1/.
* https://skypjack.github.io/2019-03-21-ecs-baf-part-2-insights/.
* https://skypjack.github.io/2019-09-25-ecs-baf-part-5/.
* https://www.computist.xyz/2018/06/sparse-sets.html.
* https://www.geeksforgeeks.org/sparse-set/.