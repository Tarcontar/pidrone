#!/bin/bash

#check if unported
if [ -d /sys/class/gpio/gpio$1 ]
then
	echo $1 < /sys/class/gpio/unexport
	sleep 1 ; #short delay while GPIO permissions are set up
	echo "unexported"
fi