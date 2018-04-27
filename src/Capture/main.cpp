#include <iostream>

#include "Camera.hpp"
#include "../Processing/Imageprocessor.hpp"

int main()
{
	Camera c("/dev/video0");
	void *shared_mem = c.capture(420, 594);

	Imageprocessor proc(shared_mem, 420, 594);
	proc.toGrey(shared_mem);

}
