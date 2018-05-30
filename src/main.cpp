#include "Capture/Camera.hpp"
#include "Gpio/Gpio.hpp"
#include "Processing/Imageprocessor.hpp"

#include <stdio.h>

int main(int argc, char *argv[])
{
	Camera c("/dev/video0");
	void *sm = c.capture(420, 594);

	Imageprocessor i(sm, 420, 594);
	std::vector<unsigned char> vec = i.toGrey(sm);


    return 0;
}
