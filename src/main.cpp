#include <stdio.h>

#include "Gpio/Gpio.hpp"
#include "Plotter/plotter.hpp"

int main(int argc, char *argv[])
{
	gpio::export_gpio(11);
	gpio::export_gpio(9);
	gpio::export_gpio(8);
	gpio::export_gpio(7);

	gpio::set_direction(11, DIR_OUT);
	gpio::set_direction(9, DIR_OUT);
	gpio::set_direction(8, DIR_OUT);
	gpio::set_direction(7, DIR_OUT);


	Plotter p;
	p.moveTo(50, 50, false);

    return 0;
}
