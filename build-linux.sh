sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install -y git curl build-essential g++-8 libxml2 libxml2-dev liballegro5.2 liballegro5-dev
curl -L -o premake5.tar.gz https://github.com/premake/premake-core/releases/download/v5.0.0-alpha13/premake-5.0.0-alpha13-linux.tar.gz
tar -xvzf premake5.tar.gz
export CC=gcc-8
export CXX=g++-8
./premake5 --cc=gcc --os=linux --file=./premake5.lua --verbose gmake