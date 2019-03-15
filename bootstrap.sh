#!/usr/bin/env bash
rm -rf pidrone
echo "Installing dependencies"
sudo apt install python git gcc-arm-none-eabi cmake openocd
echo "Cloning repository"
cd ~
git clone --recurse-submodules https://github.com/Tarcontar/pidrone.git
echo "update submodules"
cd pidrone
git submodule update
echo "Building project"
mkdir build && cd build
cmake ..
make all
echo "Finished now"
