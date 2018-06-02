#include "Capture/Camera.hpp"
#include "Gpio/Gpio.hpp"
#include "Processing/Imageprocessor.hpp"

#include <stdio.h>

int main(int argc, char *argv[])
{
	if(gpio::export_gpio(14) != 14)
		return 1;

	if(gpio::set_direction(14, DIR_IN))
		return 1;

	while(1)
	{
		printf("%d\n", (int) gpio::get_value(14));
	}



	Camera c("/dev/video0");								/* Make an Camera object (asume that the bcm2835-v4l2 module is loaded in the kernel)*/
	void *sm = c.capture(420, 594);							/* Capture an 420 x 594 image (in RGB) and save the pointer to the shared memory */

	Imageprocessor i(sm, 420, 594);							/* Make an Imageprocessor object */
	std::vector<unsigned char> grey = i.toGrey(sm);			/* Convert the RGB24 to greyscale so our next operations will succeed */

	std::vector<unsigned char> blur = i.blur(grey);			/* Do a gaussian blur so we reduce the amount of 'ruis' in next steps */
	std::vector<Imageprocessor::sobel> sobel = i.toSobel(blur);	/* Do the sobel operator to do the first edge detection */




    return 0;
}
