#!/bin/bash

echo 11 > /sys/class/gpio/export
echo 9  > /sys/class/gpio/export

echo "out" > /sys/class/gpio/gpio11/direction
echo "out" > /sys/class/gpio/gpio9/direction

echo 0 > /sys/class/gpio/gpio9/value

while true
do
	echo 1 > /sys/class/gpio/gpio11/value
	sleep 0.01
	echo 0 > /sys/class/gpio/gpio11/value
	sleep 0.01
done


