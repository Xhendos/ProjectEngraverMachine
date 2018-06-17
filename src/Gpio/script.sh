#!/bin/bash

echo 7 > /sys/class/gpio/export
echo 8 > /sys/class/gpio/export

echo "out" > /sys/class/gpio/gpio7/direction
echo "out" > /sys/class/gpio/gpio8/direction

echo 1 > /sys/class/gpio/gpio8/value

while true
do
	echo 1 > /sys/class/gpio/gpio7/value
	sleep 0.000001
	echo 0 > /sys/class/gpio/gpio7/value
	sleep 0.000001
done


