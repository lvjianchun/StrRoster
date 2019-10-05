############################################################
# Need install cmake and ninja before build this package
# apt install cmake
# apt install ninja-build
############################################################

mkdir -p build
cd build

cmake -B ../ -G Ninja
ninja

