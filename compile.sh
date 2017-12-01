#!/bin/bash
cd deps/libopencm3
make
cd ../..
mkdir -p build
cd build
cmake ..
sudo make all
