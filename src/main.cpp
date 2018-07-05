#include <stdio.h>

#include "Gpio/Gpio.hpp"
#include "Plotter/Plotter.hpp"

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


	plotter p;
	p.moveTo(400, 200, false);
	p.moveTo(600, 800, true);
	p.moveTo(200, 800, true);
	p.moveTo(400, 200, true);
	p.moveTo(20, 20, false);

    return 0;
}
