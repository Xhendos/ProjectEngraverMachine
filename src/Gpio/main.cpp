#include "gpio.hpp"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

int main()
{
    gpio::export_gpio(7);
	gpio::export_gpio(8);

	sleep(2);

	gpio::set_direction(7, DIR_OUT);
	gpio::set_direction(8, DIR_OUT);

	sleep(2);

	gpio::set_value(8, 1);

	while(1)
	{
		gpio::set_value(7, 1);
		usleep(250);
		gpio::set_value(7, 0);
		usleep(250);
	}

    return 0;
}
