#include <stdio.h>

#include <linux/reboot.h>
#include <sys/reboot.h>
#include <unistd.h>

#include "Gpio/Gpio.hpp"
#include "Processing/Imageprocessor.hpp"
#include "Capture/Camera.hpp"
#include "Plotter/Plotter.hpp"

void start(Camera &c);

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

	printf("[ENGRAVER] Finished setting up all I/O pins\n");


	Camera c("/dev/video0");


	while(1)
	{
		if(gpio::get_value(14)) reboot(LINUX_REBOOT_CMD_HALT);
		if(gpio::get_value(15)) start(c);
		sleep(1);
	}

	return 0;
}

void start(Camera &c)
{
	plotter p;

	void *sm = c.capture(800, 800);
	Imageprocessor i(sm, 800, 800);

    std::vector<unsigned char> grey = i.toGrey(sm);         /* Convert the RGB24 to greyscale so our next operations will succeed */
	std::vector<unsigned char> cropped = i.crop(grey, 17, 139, 576, 516);

	i.setSize(559, 377);

    std::vector<unsigned char> blur = i.blur(cropped);         /* Do a gaussian blur so we reduce the amount of 'ruis' in next steps */
    std::vector<Imageprocessor::sobel> sobel = i.toSobel(cropped); /* Do the sobel operator to do the first edge detection */

	int index = 0;
	for(int x = 376; x >= 0; x--)
	{
		for(int y = 0; y <= 558; y++)
		{
			p.setMap(x, y, sobel[index].m);
			index++;
		}
	}
	printf("[ENGRAVER] Set to (0,0) and press the yellow button\n");
	while(!gpio::get_value(15)) sleep(1);
	p.findLine();

	p.loadStart();

	p.run(false);
	p.loadLine();
	p.run(true);
}
