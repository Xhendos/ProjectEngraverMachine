#include <stdio.h>

#include <linux/reboot.h>
#include <sys/reboot.h>
#include <unistd.h>

#include "Gpio/Gpio.hpp"
#include "Processing/Imageprocessor.hpp"
#include "Capture/Camera.hpp"

int main(int argc, char *argv[])
{
	printf("[ENGRAVER] Setting up all I/O pins\n");

	if(gpio::export_gpio(7) != 7)							/* Step  motor x*/
		return 1;

	if(gpio::export_gpio(8) != 8)							/* Direction motor x */
		return 1;

	if(gpio::export_gpio(11) != 11)							/* Step motor y */
		return 1;

	if(gpio::export_gpio(9) != 9)							/* Direction motor y */
		return 1;

	if(gpio::export_gpio(4) != 4)							/* Yellow LED */
		return 1;

	if(gpio::export_gpio(17) != 17)							/* Green LED */
		return 1;

	if(gpio::export_gpio(14) != 14)							/* Red button (shutdown) */
		return 1;

	if(gpio::export_gpio(15) != 15)							/* Yellow button (start) */
		return 1;

	sleep(1);


	if(gpio::set_direction(7, DIR_OUT))
		return 1;

	if(gpio::set_direction(8, DIR_OUT))
		return 1;

	if(gpio::set_direction(11, DIR_OUT))
		return 1;

	if(gpio::set_direction(9, DIR_OUT))
		return 1;

	if(gpio::set_direction(4, DIR_OUT))
		return 1;

	if(gpio::set_direction(17, DIR_OUT))
		return 1;

	if(gpio::set_direction(14, DIR_IN))
		return 1;

	if(gpio::set_direction(15, DIR_IN))
		return 1;

	gpio::set_value(15, 1);

	printf("[ENGRAVER] Finished setting up all I/O pins\n");



	while(1)
	{
		if(gpio::get_value(14)) reboot(LINUX_REBOOT_CMD_HALT);
	}



	Camera c("/dev/video0");								/* Make an Camera object (asume that the bcm2835-v4l2 module is loaded in the kernel)*/
	void *sm = c.capture(420, 594);							/* Capture an 420 x 594 image (in RGB) and save the pointer to the shared memory */

	Imageprocessor i(sm, 420, 594);							/* Make an Imageprocessor object */
	std::vector<unsigned char> grey = i.toGrey(sm);			/* Convert the RGB24 to greyscale so our next operations will succeed */

	std::vector<unsigned char> blur = i.blur(grey);			/* Do a gaussian blur so we reduce the amount of 'ruis' in next steps */
	std::vector<Imageprocessor::sobel> sobel = i.toSobel(blur);	/* Do the sobel operator to do the first edge detection */

	return 0;
}
