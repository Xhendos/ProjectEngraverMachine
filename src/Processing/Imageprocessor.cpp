#include <vector>
#include "Imageprocessor.hpp"
#include <stdio.h>

Imageprocessor::Imageprocessor(void *sm, int w, int h)
{
	shared_mem = sm;
	width = w;
	height = h;
}

std::vector<unsigned char> Imageprocessor::toGrey(void *start)
{
	std::vector<unsigned char> grey;

	unsigned char r;
	unsigned char g;
	unsigned char b;

	printf("Te doen is %d\n", width * height);

	for(int index = 0; index < (width*height); index += 3)
	{
		r = *((char *) start + index);
		g = *((char *) start + (index + 1));
		b = *((char *) start + (index + 2));

		grey.push_back(((r + g + b) / 3));
	}
		
	printf("%d\n", grey.size());

	return grey;
}
