brew update
brew install --force allegro
brew install --force libxml2
brew install --force curl
sudo curl -L -o premake5.tar.gz https://github.com/premake/premake-core/releases/download/v5.0.0-alpha13/premake-5.0.0-alpha13-macosx.tar.gz
sudo tar -xvzf premake5.tar.gz
sudo ./premake5 --cc=clang --os=macosx --file=premake5.lua --verbose gmake