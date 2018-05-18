#include "gpio.hpp"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

int main()
{
    gpio::gpio_header h;
    h.pin = 11;
    h.direction = DIR_IN;
    h.value = 1;
    h.edge = EDGE_NONE;
    h.active_low = 0;

    gpio::export_gpio(h);

    sleep(15);

    return 0;
}
