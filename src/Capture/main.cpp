#include <iostream>

#include "Camera.hpp"

int main()
{
	Camera c("/dev/video0");
	c.capture(124, 46);
}
