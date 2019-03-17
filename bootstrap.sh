#!/usr/bin/env bash
sudo apt-get update
sudo apt-get upgrade
rm -rf pidrone
echo "Installing dependencies"
sudo apt install python python-pip git gcc-arm-none-eabi cmake autoconf libtool make pkg-config libusb-1.0-0 libusb-1.0-0-dev
pip install pyserial
echo "build openocd"
rm -rf openocd
git clone http://openocd.zylin.com/openocd
cd openocd
./bootstrap
./configure --enable-sysfsgpio --enable-bcm2835gpio
make
sudo make install
echo "Cloning repository"
cd ~
git clone --recurse-submodules https://github.com/Tarcontar/pidrone.git
echo "Building project"
cd pidrone
mkdir build
cd build
cmake ..
make all
echo "Finished now"
