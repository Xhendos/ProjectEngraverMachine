#include <iostream>

int main()
{

	return 0;
}


std::vector<Imageprocessor::sobel> Imageprocessor::sobelOperator(std::vector<unsigned char> grey)
{
    std::vector<Imageprocessor::sobel sobels;                              /* Vector to hold all sobel data structures for the new image */

    for(int index = 0; index < grey.size(); index++)                        
    {
        if(index < width)                                                   /* Exception for the first row*/    
        {
            struct sobel s = {0, 0, 0, 0};
            sobels.push_back(s);

            continue;
        }  

        if(index > (height - 1) * width)                                    /* Exception for the last row */
        { 
            struct sobel s = {0, 0, 0, 0};
            sobels.push_back(s);

            continue;
        } 

        if(!(index % width))                                                /* Exception for the first column */
        {
            struct sobel s = {0, 0, 0, 0};
            sobels.push_back(s);

            continue;
        } 

        if(!((index + 1) % width))                                          /* Exception for the last column */
        {
            struct sobel s = {0, 0, 0, 0};
            sobels.push_back(s);

            continue;
        }

        struct sobel s;
        s.gx =  ((grey[index - 1] * -2) +                                   /* I(u-1, v)    */
                (grey[index - width - 1] * -1) +                            /* I(u-1, v-1)  */
                (grey[index + width - 1] * -1) +                            /* I(u-1, v+1)  */  
                (grey[index + 1] * 2) +                                     /* I(u+1, v)    */
                (grey[index + width + 1]) +                                 /* I(u+1, v+1)  */
                (grey[index - width + 1]));                                 /* I(u+1, v-1)  */
        
        s.gy =  ((grey[index - width] * -2) +                               /* I(u, v-1)    */
                (grey[index - width - 1] * -1) +                            /* I(u-1, v-1)  */
                (grey[index - width + 1] * -1) +                            /* I(u+1, v-1)  */
                (grey[index + width] * 2) +                                 /* I(u, v+1)    */
                (grey[index + width - 1]) +                                 /* I(u-1, v+1)  */
                (grey[index + width + 1]));                                 /* I(u+1, v+1)  */

        s.m = sqrt(s.gx * s.gx + s.gy * s.gy);                              /* magnitude = square root(input[i].aient_x^2 + input[i].aient_y^2) */

        s.a = atan2((double) s.gy, (double) s.gx) * 180.0 / M_PI;           /* angle = tan^(-1) (input[i].aient_y / input[i].aient_x) */       
                                                                            /* degree = angle * 180 / PI */
        sobels.push_back(s);
    }


std::vector<Imageprocessor::sobel> Imageprocessor::canny(std::vector<Imageprocessor::sobel> input)
{
	std::vector<Imageprocessor::sobel> output;

	for(int i = 0; i < input.size(); i++)		//making the angle region a specific angle.
	{	
		if (input[i].a >= 157.5 && input[i].a <= 180 | input[i].a >= 0 && input[i].a <= 22.4)
		{
			input[i].a = 0;
		}
		if (input[i].a >= 22.5 && input[i].a <= 67.4)
		{
			input[i].a = 45;
		}
		if (input[i].a >= 67.5 && input[i].a <= 112.4)
		{
			input[i].a = 90;
		}
		if (input[i].a >= 112.5 && input[i].a <= 157.4)
		{
			input[i].a = 135;
		}

		output[i].a = input[i].a;

		if (output[i].a == 0)
		{
			if (i < width | i > (height - 1) * width | !(index % width) | !((i + 1) % width)) //if the targetted pixel is on the edge... (applies to all grad options)
			{
				struct sobel s = {0, 0, 0, 0};
				output.push_back(s);
				continue;
			}
			else
			{
				*0, *1, *0, *0, *1, *0, *0, *1, *0//do some weird shit vertical
			}
		}	

		if (output[i].a == 45)
		{
			if (i < width | i > (height - 1) * width | !(index % width) | !((i + 1) % width))
			{
				struct sobel s = {0, 0, 0, 0};
				output.push_back(s);
				continue;
			}
			else
			{
				*0, *0, *1, *0, *1, *0, *1, *0, *0//do some weird shit diagonally to the right
			}
	
		}
		if (output[i].a == 90)
		{
			if (i < width | i > (height - 1) * width | !(index % width) | !((i + 1) % width))
			{
				struct sobel s = {0, 0, 0, 0};
				output.push_back(s);
				continue;
			}
			else
			{
				*0, *0, *0, *1, *1, *1, *0, *0, *0//do some weird shit horizontally
			}
	
		}
		if (output[i].a == 135)
		{
			if (i < width | i > (height - 1) * width | !(index % width) | !((i + 1) % width))
			{
				struct sobel s = {0, 0, 0, 0};
				output.push_back(s);
				continue;
			}
			else
			{
				*1, *0, *0, *0, *1, *0, *0, *0, *1//do some weird shit diagonally to the left
			}	
		}
	}
	

	return output;
}
