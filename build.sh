sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install -y curl build-essential g++-8 libxml2 libxml2-dev liballegro5.2 liballegro5-dev
curl -L -o premake5.tar.gz https://github.com/premake/premake-core/releases/download/v5.0.0-alpha13/premake-5.0.0-alpha13-linux.tar.gz
tar -xvzf premake5.tar.gz
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 800 --slave /usr/bin/g++ g++ /usr/bin/g++-8
./premake5 --cc=gcc --os=linux --file=premake5.lua --verbose gmake