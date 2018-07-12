#include <vector>
#include "Imageprocessor.hpp"
#include <stdio.h>
#include <SFML/Graphics.hpp>

#include <math.h>                                                           /* sqrt(), M_PI, atan2() */

#define HIGHTRESH 200
#define LOWTRESH HIGHTRESH/2

Imageprocessor::Imageprocessor(void *sm, unsigned int w, unsigned int h)
{
	shared_mem = sm;
	width = w;
	height = h;
}

void Imageprocessor::setSize(unsigned int w, unsigned int h)
{
	width = w;
	height = h;
}

std::vector<unsigned char> Imageprocessor::crop(std::vector<unsigned char> pixels, unsigned int x_left, unsigned int y_left, unsigned int x_right, unsigned int y_right)
{

	//char *ret = (char *) malloc((x_right - x_left) * (y_right - y_left));
	std::vector<unsigned char> ret;
	int x_original, y_original, counter = 0;

	for(unsigned int index = 1; index <= (width * height); index++)
	{
		x_original = index % width;
		y_original = (index / width) + 1;

		if((x_original >= x_left && x_original < x_right) && (y_original >= y_left && y_original < y_right))
		{
			// *((char *) ret + counter) = pixels[index - 1];
			ret.push_back(pixels[index - 1]);
			counter++;
		}
	}

	printf("Counter is %d\n", counter);
	/*
	int _x = 0, _y = 0;

	for(int i = 1; i <= 800; i++)
	{
		for(int j = 1; j <= 800; j++)
		{
			if((i * j) > ((800 * y_left) - 1) && (i*j) <= (800 * y_right))
			{
				if(j >= x_left && j <= x_right)
				{
					*((char *) ret + counter) = *((char *) pixels + ((i * j) -1));
           			counter++;
				}
			}
		}
	}
	*/

	sf::Image image;
    image.create((x_right - x_left), (y_right - y_left));
    for(int y=0; y<(y_right - y_left); y++){
        for(int x=0; x<(x_right - x_left); x++){
            int index=(y*(x_right - x_left) + x);
            //sf::Color c(*((char *) ret + index), *((char *) ret + index), *((char *)ret + index));
            sf::Color c(ret[index], ret[index], ret[index]);
			image.setPixel(x,y,c);
        }
    }

    image.saveToFile("GREYpicture6.png");



	return ret;
}


std::vector<unsigned char> Imageprocessor::toGrey(void *start)
{
	std::vector<unsigned char> grey;										/* Imagevector where all the greyscale values will sit */

	unsigned char r;														/* red intensity */
	unsigned char g;														/* green intensity */
	unsigned char b;														/* blue intensity */


	for(unsigned int index = 0; index < (width * height * 3); index += 3)
	{
		r = *((char *) start + index);
		g = *((char *) start + (index + 1));
		b = *((char *) start + (index + 2));

		grey.push_back(((r + g + b) / 3));									/* Push the average of the rgb to the vector (average is greyscale) */
	}


    sf::Image image;
    image.create(width, height);
    for(int y=0; y<height; y++){
        for(int x=0; x<width; x++){
            int index=(y*width + x);
            sf::Color c(grey[index], grey[index], grey[index]);
            image.setPixel(x,y,c);
        }
	}

	image.saveToFile("GREYpicture.png");

	return grey;
}



std::vector<unsigned char> Imageprocessor::blur(std::vector<unsigned char> grey)
{
	std::vector<unsigned char> blurred;										/* Vector that holds the results of the gaussian blur
																			 * and values that could not be blurred (edges of an image) */

	for(unsigned int index = 0; index < grey.size(); index++)
	{
		if(index < (2 * width))												/* Exception for the first two rows */
		{
			blurred.push_back(grey[index]);
			continue;
		}

		if(index >= ((height - 2) * width))									/* Exception for the last two rows */
		{
			blurred.push_back(grey[index]);
			continue;
		}

		if(!(index % width))												/* Exception for the first element of each row */
		{
			blurred.push_back(grey[index]);
			continue;
		}

		if(!((index - 1) % width))											/* Exception for the last element of each row */
		{
			blurred.push_back(grey[index]);
			continue;
		}

		if(!((index + 1) % width))											/* Exception for the second element of each row */
		{
			blurred.push_back(grey[index]);
			continue;
		}

		if(!((index - 2) % width))											/* Exception of the second last element of each row */
		{
			blurred.push_back(grey[index]);
			continue;
		}

																			/* Do a Gaussian blur and store the result in b */
		unsigned char b = 	((41 * grey[index]) + 							/* I(u, v) 		*/
							(26 * grey[index - 1]) +						/* I(u-1, v)	*/
							(26 * grey[index + 1]) +						/* I(u+1, v) 	*/
							(26 * grey[index - width]) +					/* I(u, v-1) 	*/
							(26 * grey[index + width]) +					/* I(u, v+1)	*/
							(16 * grey[index - 1 - width]) +				/* I(u-1, v-1)	*/
							(16 * grey[index + 1 - width]) +				/* I(u+1, v-1)	*/
							(16 * grey[index - 1 + width]) +				/* I(u-1, v+1)	*/
							(16 * grey[index + 1 + width]) +				/* I(u+1, v+1)	*/
							(7  * grey[index - (2 * width)]) +				/* I(u, v-2)	*/
							(7  * grey[index + (2 * width)]) +				/* I(u, v+2)	*/
							(7  * grey[index - 2]) +						/* I(u-2, v)	*/
							(7  * grey[index + 2]) +						/* I(u+2, v)	*/
							(4  * grey[index - (2 * width) - 1]) +			/* I(u-1, v-2)	*/
							(4  * grey[index - (2 * width) + 1]) +			/* I(u+1, v-2)	*/
							(4  * grey[index + (2 * width) - 1]) +			/* I(u-1, v+2)	*/
							(4  * grey[index + (2 * width) + 1]) +			/* I(u+1, v-2)	*/
							(4  * grey[index - width - 2]) +				/* I(u-2, v-1)	*/
							(4  * grey[index - width + 2]) +				/* I(u+2, v-1)	*/
							(4  * grey[index + width - 2]) +				/* I(u-2, v+1)	*/
							(4  * grey[index + width + 2]) +				/* I(u+2, v+1)	*/
							(grey[index - (2 * width) - 2]) +				/* I(u-2, v-2)	*/
							(grey[index - (2 * width) + 2]) +				/* I(u+2, v-2)	*/
							(grey[index + (2 * width) - 2]) +				/* I(u-2, v+2)	*/
							(grey[index + (2 * width) + 2])) / 271;			/* I(u+2, v+2)	*/


		blurred.push_back(b);												/* add the blurred pixel to the new imagevector */

	}

    sf::Image image;
    image.create(width, height);
    for(int y=0; y<height; y++){
        for(int x=0; x<width; x++){
            int index=(y*width + x);
            sf::Color c(grey[index], grey[index], grey[index]);
            image.setPixel(x,y,c);
        }
    }


	image.saveToFile("GREYpicture2.png");


	return blurred;
}


std::vector<Imageprocessor::sobel> Imageprocessor::toSobel(std::vector<unsigned char> grey)
{
    std::vector<Imageprocessor::sobel> sobels;                              /* Vector to hold all sobel data structures for the new image */
	std::vector<unsigned char> edges;

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

        s.m = sqrt(s.gx * s.gx + s.gy * s.gy);                              /* magnitude = square root(gradient_x^2 + gradient_y^2) */

        s.a = atan2((double) s.gy, (double) s.gx) * 180.0 / M_PI;           /* angle = tan^(-1) (gradient_y / gradient_x) */       
                                                                            /* degree = angle * 180 / PI */
        sobels.push_back(s);
    	edges.push_back(s.m);
	}

    
	sf::Image image;
  	image.create(width, height);
  	for(int y=0; y<height; y++){
  		for(int x=0; x<width; x++){
           	int index=(y*width + x);
         	sf::Color c(sobels[index].m, sobels[index].m, sobels[index].m);
          	image.setPixel(x,y,c);
        }
  	}

	image.saveToFile("GREYpicture3.png");


	//Imageprocessor::canny(sobels, edges);

    return sobels;
}



std::vector<Imageprocessor::sobel> Imageprocessor::nonmax_suppression(std::vector<Imageprocessor::sobel> &pixels)
{
	std::vector<Imageprocessor::sobel> ret;


	for(unsigned int index = 0; index < (width * height); index++)
	{
		if(index < width)                                                   /* Exception for the first row*/    
        {
            struct sobel s = {0, 0, 0, 0};
            ret.push_back(s);

            continue;
        }

        if(index > (height - 1) * width)                                    /* Exception for the last row */
        {
            struct sobel s = {0, 0, 0, 0};
            ret.push_back(s);

            continue;
        }

        if(!(index % width))                                                /* Exception for the first column */
        {
            struct sobel s = {0, 0, 0, 0};
            ret.push_back(s);

            continue;
        }

        if(!((index + 1) % width))                                          /* Exception for the last column */
        {
            struct sobel s = {0, 0, 0, 0};
            ret.push_back(s);

            continue;
        }


																			/* 0 degrees*/
		if((pixels[index].a >= 337.5 || pixels[index].a < 22.5) || (pixels[index].a >= 157.5 && pixels[index].a < 202.5))
		{																	/* If we found the strongest edge */
			if(pixels[index].m >= pixels[index + 1].m && pixels[index].m >= pixels[index - 1].m)
			{
				pixels[index - 1].m = 0;
				pixels[index + 1].m = 0;
			}
		}
																			/* 45 degrees*/
		if((pixels[index].m >= 22.5 && pixels[index].m < 67.5) || (pixels[index].m >= 202.5 && pixels[index].m < 247.5))
		{																	/* If we found the strongest edge */
			if(pixels[index].m >= pixels[(index - width) + 1].m && pixels[index].m >= pixels[(index + width) - 1].m)
			{
				pixels[(index - width) + 1].m = 0;
				pixels[(index + width) - 1].m = 0;
			}
		}
																			/* 90 degrees */
		if((pixels[index].m >= 67.5 && pixels[index].m <= 112.5) || (pixels[index].m >= 247 && pixels[index].m < 292.5))
		{																	/* If we found the strongest edge*/
			if(pixels[index].m >= pixels[index - width].m && pixels[index].m >= pixels[index + width].m)
			{
				pixels[index - width].m = 0;
				pixels[index + width].m = 0;
			}
		}


	}

    sf::Image image;
    image.create(width, height);
    for(int y=0; y<height; y++){
        for(int x=0; x<width; x++){
            int index=(y*width + x);
            sf::Color c(pixels[index].m, pixels[index].m, pixels[index].m);
            image.setPixel(x,y,c);
        }
    }

	image.saveToFile("GREYimage4.png");


	return ret;
}

/* https://stackoverflow.com/questions/35238047/how-do-i-interpret-the-orientation-of-the-gradient-when-using-imgradient-in-matl */
/* https://stackoverflow.com/questions/19815732/what-is-gradient-orientation-and-gradient-magnitude */



