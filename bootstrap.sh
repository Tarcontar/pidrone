#!/usr/bin/env bash
echo "Installing dependencies"
sudo apt install git gcc-arm-none-eabi cmake openocd
echo "Cloning repository"
cd ~
git clone --recursive https://github.com/Tarcontar/pidrone.git
cd pidrone
echo "Building project"
mkdir build && cd build 
cmake .. -DCMAKE_TOOLCHAIN_FILE="../cmake/stm32-toolchain.cmake"
cmake --build .
echo "Finished now"