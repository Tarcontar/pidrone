#!/usr/bin/env bash
rm -rf pidrone
echo "Installing dependencies"
sudo apt install python git gcc-arm-none-eabi cmake openocd
echo "Cloning repository"
cd ~
git clone --recursive https://github.com/Tarcontar/pidrone.git
echo "update submodules"
cd pidrone/contrib/tinygps
git checkout master
git pull 
cd ../NemaTode
git checkout master
git pull
cd ../bosch/bme280
git checkout master
git pull
cd ../bmi160
git checkout master
git pull
cd ../../libopencm3
git checkout master
git pull
cd ../..
echo "Building project"
mkdir build && cd build 
cmake .. -DCMAKE_TOOLCHAIN_FILE="../cmake/stm32-toolchain.cmake"
cmake --build .
echo "Finished now"
