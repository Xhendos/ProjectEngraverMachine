#include <math.h>
#include <stdlib.h>

void extent(sobel) //omdat de previous-made extent() n^2 tijd duurde
{ //(oppervlakte / omtrek) * 4pi
	int bovenstepunt;
	int onderstepunt;
	double diameter, omtrek;
	
	for(int i = 0; sobel[i].m == 0; i++;)
	{
		bovenstepunt = i;
	}
	
	for(int i = sobel.size(); sobel[i].m == 0; i--)
	{
		onderstepunt = i;
	}
	
	diameter = onderstepunt - bovenstepunt;
	
	return (((oppervlakte / omtrek) * (4 * M_PI))); //#define M_PI 3.14159265358979323846 inside of math.h
}