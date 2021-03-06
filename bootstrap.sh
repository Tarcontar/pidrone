#!/usr/bin/env bash
echo "Installing dependencies"
sudo apt install python python-pip git gcc-arm-none-eabi cmake autoconf libtool make pkg-config libusb-1.0-0 libusb-1.0-0-dev
pip install pyserial
echo "build openocd"
cd ~
rm -rf openocd
git clone http://openocd.zylin.com/openocd
cd openocd
./bootstrap
./configure --enable-sysfsgpio --enable-bcm2835gpio
make
sudo make install
echo "Cloning repository"
cd ~
rm -rf pidrone
git clone --recursive https://github.com/Tarcontar/pidrone.git
echo "Building project"
cd pidrone
mkdir build
cd build
cmake ..
make all
echo "Finished"
echo "Make sure you enabled USART with 'sudo raspbi-config'"
