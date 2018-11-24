brew update
brew install allegro
brew install libxml2
brew install curl
curl -L -o premake5.tar.gz https://github.com/premake/premake-core/releases/download/v5.0.0-alpha13/premake-5.0.0-alpha13-macosx.tar.gz
tar -xvzf premake5.tar.gz
./premake5 --cc=clang --os=macosx --file=./premake5.lua --verbose gmake
make config=debug_macosx
make config=release_macosx