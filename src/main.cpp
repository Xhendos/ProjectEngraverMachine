#include "Capture/Camera.hpp"
#include "Gpio/Gpio.hpp"
#include "Processing/Imageprocessor.hpp"
#include <stdlib.h>
#include <stdio.h>

#include <linux/reboot.h>
#include <sys/reboot.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int opp = 500;

	/* Export used I/O pins */
	if(!gpio::export_gpio(3))								/* Green status LED */
		exit(1);
	if(!gpio::export_gpio(4))								/* Yellow status LED */
		exit(1);
	if(!gpio::export_gpio(14))								/* Red button */
		exit(1);
	if(!gpio::export_gpio(15))								/* Yellow button */
		exit(1);
	if(!gpio::export_gpio(11))								/* Step motor a */
		exit(1);
	if(!gpio::export_gpio(9))								/* Direction motor a */
		exit(1);
	if(!gpio::export_gpio(7))								/* Step motor b */
		exit(1);
	if(!gpio::export_gpio(8))								/* Direction motor b */
		exit(1);



	if(gpio::set_direction(3, DIR_OUT))
		exit(1);
	if(gpio::set_direction(4, DIR_OUT))
		exit(1);
	if(gpio::set_direction(14, DIR_IN))
		exit(1);
	if(gpio::set_direction(15, DIR_IN))
		exit(1);
	if(gpio::set_direction(11, DIR_OUT))
		exit(1);
	if(gpio::set_direction(9, DIR_OUT))
		exit(1);
	if(gpio::set_direction(7, DIR_OUT))
		exit(1);
	if(gpio::set_direction(8, DIR_OUT))
		exit(1);


//	while(1)
//	{
//		if(gpio::get_value(14)) reboot(LINUX_REBOOT_CMD_HALT);
//		if(gpio::get_value(14)) reboot(LINUX_REBOOT_CMD_HALT);
//	}

	Camera c("/dev/video0");								/* Make an Camera object (asume that the bcm2835-v4l2 module is loaded in the kernel)*/
	void *sm = c.capture(800, 800);							/* Capture an 420 x 594 image (in RGB) and save the pointer to the shared memory */

	Imageprocessor i(sm, 800, 800);							/* Make an Imageprocessor object */
	std::vector<unsigned char> grey = i.toGrey(sm);			/* Convert the RGB24 to greyscale so our next operations will succeed */

	std::vector<unsigned char> blur = i.blur(grey);			/* Do a gaussian blur so we reduce the amount of 'ruis' in next steps */
	std::vector<Imageprocessor::sobel> sbl = i.toSobel(blur);	/* Do the sobel operator to do the first edge detection */
	//std::vector<Imageprocessor::sobel> nms = i.nonmax_suppression(sbl);
/*
	for(int index = 0; index <= sbl.size(); index++)
	{
		if (i.crossedge(index, sbl))
		{
			opp++;
			printf("er is een punt gevonden binnen het polygoon. counter is %i.\n" , opp);
		}
	}
*/
	printf("\n Opp = %i\n", opp);
	i.extent(opp, sbl);

    return 0;
}
