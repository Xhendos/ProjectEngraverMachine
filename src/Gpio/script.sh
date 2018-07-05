#!/bin/bash

echo 7 > /sys/class/gpio/export
echo 8 > /sys/class/gpio/export

echo 11 > /sys/class/gpio/export
echo 9 > /sys/class/gpio/export

echo "out" > /sys/class/gpio/gpio7/direction
echo "out" > /sys/class/gpio/gpio8/direction

echo "out" > /sys/class/gpio/gpio11/direction
echo "out" > /sys/class/gpio/gpio9/direction

echo 1 > /sys/class/gpio/gpio8/value
echo 1 > /sys/class/gpio/gpio9/value

while true
do
	echo 1 > /sys/class/gpio/gpio7/value
	echo 1 > /sys/class/gpio/gpio11/value
	sleep 0.001
	echo 0 > /sys/class/gpio/gpio7/value
	echo 0 > /sys/class/gpio/gpio11/value
	sleep 0.001
done


