/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

brew update
brew install allegro
brew install libxml2
brew install curl
git clone --recursive --branch=master https://github.com/reworks/starlight.git reworks/starlight
cd reworks/starlight
curl -L -o premake5.tar.gz https://github.com/premake/premake-core/releases/download/v5.0.0-alpha13/premake-5.0.0-alpha13-macosx.tar.gz
tar -xvzf premake5.tar.gz
./premake5 --cc=clang --os=macosx --file=./premake5.lua --verbose gmake
make config=debug_macosx
make config=release_macosx