#include <iostream>

#include "Camera.hpp"
#include "../Processing/Imageprocessor.hpp"
#include <vector>

int main()
{
	Camera c("/dev/video0");
	void *shared_mem = c.capture(420, 594);

    std::cout << sizeof(short int) << std::endl;

	Imageprocessor proc(shared_mem, 420, 594);
	std::vector<unsigned char> grey = proc.toGrey(shared_mem);
	std::vector<unsigned char> blur = proc.blur(grey);
    proc.sobelOperator(blur);
    

	return 0;
}
