
<<<<<<< HEAD
#include <stdio.h>

int main(int argc, char *argv[])
{
	Camera c("/dev/video0");								/* Make an Camera object (asume that the bcm2835-v4l2 module is loaded in the kernel)*/
	void *sm = c.capture(420, 594);							/* Capture an 420 x 594 image (in RGB) and save the pointer to the shared memory */

	Imageprocessor i(sm, 420, 594);							/* Make an Imageprocessor object */
	std::vector<unsigned char> grey = i.toGrey(sm);			/* Convert the RGB24 to greyscale so our next operations will succeed */

	std::vector<unsigned char> blur = i.blur(grey);			/* Do a gaussian blur so we reduce the amount of 'ruis' in next steps */
	std::vector<Imageprocessor::sobel> sbl = i.toSobel(blur);	/* Do the sobel operator to do the first edge detection */
	std::vector<Imageprocessor::sobel> nms = i.nonmax_suppression(sbl);


    return 0;
}
=======
>>>>>>> 725270e3c585bf5ebbccd52c7ef850d1b17e5f7c
