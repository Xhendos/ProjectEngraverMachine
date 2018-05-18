#include <vector>
#include "Imageprocessor.hpp"
#include <stdio.h>
#include <SFML/Graphics.hpp>

#include <math.h>                                                           /* sqrt(), M_PI, atan() */

Imageprocessor::Imageprocessor(void *sm, unsigned int w, unsigned int h)
{
	shared_mem = sm;
	width = w;
	height = h;
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
    image.create(420, 594);
    for(int y=0; y<594; y++){
        for(int x=0; x<420; x++){
            int index=(y*420 + x);
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
  	image.create(420, 594);
  	for(int y=0; y<594; y++){
  		for(int x=0; x<420; x++){
           	int index=(y*420 + x);
         	sf::Color c(blurred[index], blurred[index], blurred[index]);
          	image.setPixel(x,y,c);
        }
  	}

	image.saveToFile("GREYpicture2.png");


	return blurred;
}


std::vector<Imageprocessor::sobel> Imageprocessor::sobelOperator(std::vector<unsigned char> grey)
{
    std::vector<Imageprocessor::sobel> sobels;                              /* Vector to hold all sobel data structures for the new image */

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

        s.a = atan((double) s.gy / (double) s.gx) * 180.0 / M_PI;           /* angle = tan^(-1) (gradient_y / gradient_x) */       
                                                                            /* degree = angle * 180 / PI */
        sobels.push_back(s);
    }

    
	sf::Image image;
  	image.create(420, 594);
  	for(int y=0; y<594; y++){
  		for(int x=0; x<420; x++){
           	int index=(y*420 + x);
         	sf::Color c(sobels[index].m, sobels[index].m, sobels[index].m);
          	image.setPixel(x,y,c);
        }
  	}

	image.saveToFile("GREYpicture3.png");
    return sobels;
}





