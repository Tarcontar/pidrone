#!/bin/bash
unexport ()
{
	echo "unexport $1"
	echo "$1" > /sys/class/gpio/unexport
}
unexport 18
unexport 11
unexport 10
unexport 9
bash compile.sh
cd build
make upload_drone_hat
#set Reset pin high
echo "18" > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio18/direction
echo "1" > /sys/class/gpio/gpio18/value
