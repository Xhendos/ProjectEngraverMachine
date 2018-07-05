#include <vector>
#include "Imageprocessor.hpp"
#include <stdio.h>
#include <SFML/Graphics.hpp>

#include <math.h>                                                           /* sqrt(), M_PI, atan2() */

#define HIGHTRESH 200
#define LOWTRESH HIGHTRESH/2

enum boolean {false, true};

Imageprocessor::Imageprocessor(void *sm, unsigned int w, unsigned int h)
{
	shared_mem = sm;
	width = w;
	height = h;
}

std::vector<unsigned char> Imageprocessor::canny(std::vector<Imageprocessor::sobel> input, std::vector<unsigned char> grey)
{
	std::vector<Imageprocessor::sobel> output;

	for(int i = 0; i < grey.size(); i++)		//making the angle region a specific angle.
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


		struct sobel s = input.at(i);
		output.push_back(s);


		if (output[i].a == 0)
		{
			if (i < width | i > (height - 1) * width | !(i % width) | !((i + 1) % width)) //if the targetted pixel is on the edge... (applies to all grad options) weet niet of dit nodig is.
			{
				//struct sobel s = {0, 0, 0, 0};
				//output.push_back(s);
				continue;
			}
			else
			{
				/* https://www.youtube.com/watch?v=KR_zsIKpX28
				1 2 3
				4 5 6
				7 8 9				 
				
				grey[((i-1)-width)] 								//1
				grey[(i - width)] 									//2
				grey[(i+1-width)] 									//3
				grey[(i-1)]											//4
				grey[i]												//5
				grey[(i+1)]											//6
				grey[((i-1)+width)]									//7
				grey[(i+width)] 									//8
				grey[((i+1)+width)]									//9
				
				*/
				//0 degrees == horizonal filtering to check the vertical lines
				
				if (grey[(i-1)] >= grey[(i)] && grey[(i-1)] >= grey[(i+1)])
				{
					grey[(i+1)] = grey[(i+1)] * 0;
					grey[(i)] = grey[(i)] * 0;
				}
				if (grey[(i)] >= grey[(i-1)] && grey[(i)] >= grey[(i+1)])
				{
					grey[(i+1)] = grey[(i+1)] * 0;
					grey[(i-1)] = grey[(i-1)] * 0;
				}
				if (grey[(i+1)] >= grey[(i)] && grey[(i+1)] >= grey[(i-1)])
				{
					grey[(i)] = grey[(i+1)] * 0;
					grey[(i-1)] = grey[(i-1)] * 0;
				}
			}
		}	

		if (output[i].a == 45)
		{
			if (i < width | i > (height - 1) * width | !(i % width) | !((i + 1) % width))
			{
				//struct sobel s = {0, 0, 0, 0};
				//output.push_back(s);
				continue;
			}
			else
			{
				//grey[((i-1)-width)]
				//grey[i]
				//grey[((i+1)+width)]//9
				
				if (grey[((i+1)+width)] >= grey[(i)] && grey[((i+1)+width)] >= grey[((i-1)-width)])
				{
					grey[(i+1)] = grey[(i+1)] * 0;
					grey[(i)] = grey[(i)] * 0;
				}
				if (grey[(i)] >= grey[(i-1)] && grey[(i)] >= grey[(i+1)])
				{
					grey[(i+1)] = grey[(i+1)] * 0;
					grey[(i-1)] = grey[(i-1)] * 0;
				}
				if (grey[(i+1)] >= grey[(i)] && grey[(i+1)] >= grey[(i-1)])
				{
					grey[(i)] = grey[(i+1)] * 0;
					grey[(i-1)] = grey[(i-1)] * 0;
				}
			}
	
		}
		if (output[i].a == 90)
		{
			if (i < width | i > (height - 1) * width | !(i % width) | !((i + 1) % width))
			{
				//struct sobel s = {0, 0, 0, 0};
				//output.push_back(s);
				continue;
			}
			else
			{
				if (grey[(i - width)] >= grey[i] && grey[(i - width)] >= grey[(i+width)])
				{
					grey[i] = grey[i]*0;
					grey[(i+width)] = grey[(i+width)]*0;
				}
				
				if (grey[i] >= grey[(i - width)] && grey[(i)] >= grey[(i+width)])
				{
					grey[(i+width)] = grey[(i+width)]*0;
					grey[(i - width)] = grey[(i - width)]*0;
				}
				
				if (grey[(i+width)] >= grey[(i - width)] && grey[(i+width)] >= grey[i])
				{
					grey[(i - width)] = grey[(i - width)]*0;
					grey[i] = grey[i]*0;
				}
			}
	
		}
		if (output[i].a == 135)
		{
			if (i < width | i > (height - 1) * width | !(i % width) | !((i + 1) % width))
			{
				//struct sobel s = {0, 0, 0, 0};
				//output.push_back(s);
				continue;
			}
			else
			{
				if (grey[(i+1-width)] >= grey[i] && grey[(i+1-width)] >= grey[((i-1)+width)])
				{
					grey[i] = grey[i]*0;
					grey[((i-1)+width)] = grey[((i-1)+width)]*0;
				}
				
				if (grey[i] >= grey[(i+1-width)] && grey[i] >= grey[((i-1)+width)])
				{
					grey[(i+1-width)] = grey[(i+1-width)]*0;
					grey[((i-1)+width)] = grey[((i-1)+width)]*0;
				}
				
				if (grey[((i-1)+width)] >= grey[(i+1-width)] && grey[((i-1)+width)] >= grey[i])
				{
					grey[(i+1-width)] = grey[(i+1-width)]*0;
					grey[i] = grey[i]*0;
				}
			}	
		}
		
		for(int i = 0; i <= grey.size(); i++)
		{
			if(grey[i] > HIGHTRESH) //boven treshold, i == index
			{
				grey[i] = grey[i];
				int k;
				if(grey[i-1] >= LOWTRESH && grey[i-1] <= HIGHTRESH)
				{
					k = i-1;
					while(grey[k] > LOWTRESH) //keur alle vastzittende waardes goed die boven de lowertreshold komen
					{
						grey[k] = grey[k] * -1;
						if(k == 0)
						{
							break;
						}
						k--;
					}
				}
				
				if(grey[i+1] >= LOWTRESH && grey[i+1] <= HIGHTRESH)
				{
					k = i+1;
					while(grey[k] > LOWTRESH) //keur alle vastzittende waardes goed die boven de lowertreshold komen
					{
						grey[k] = grey[k];
						if(k == grey.size())
						{
							break;
						}
						k++;
					}
					i=k; //verdergaan vanaf de plek waar die gestopt is.
				}
			}
			
			if(grey[i] >= LOWTRESH && grey[i])
			{
				grey[i] = grey[i] * -1;
			}
			
			if(grey[i] < LOWTRESH)
			{
				grey[i] = 0;
			}
		}
		
		for(int i = 0; i <= grey.size(); i++)
		{
		if(grey[i] < 0)
			{
				grey[i] = 0;
			}
		}
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

	image.saveToFile("GREYpicture4.png");

	return grey;
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
         	sf::Color c(blurred[index], blurred[index], blurred[index]);
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




int crossedge(int index, std::vector<Imageprocessor::sobel> sobel)
{
	int cn; //teller
	
	if ((sobel[index].magnitude)) //check voor witte ranged
		cn++;

	if (cn&1)
		return true;

	return false;
}

void extent(int opp, std::vector<Imageprocessor::sobel> sobel)
{
	int prevhighestX, currentX, prevhighestY, currentY;
	
	double area;

	for(int index; index <= sobel.size(); index++)
	{
		if(sobel[index].magnitude == 0)
		{
			index++;

			int indX = index;

			while(sobel[indX].magnitude != 0)
			{
				currentX++;
				indX++;
			}
			if(currentX > prevhighestX)
			{
				prevhighestX = currentX;
			}

			while(sobel[(indX + (currentYfactor * width))].magnitude != 0)
			{
				currentY++;
				currentYfactor++;
			}
			if(currentY > prevhighestY)
			{
				prevhighestY = currentY;
			}

		}
	}
	
	area = ((prevhighestX * prevhighestY)/opp);

	if(prevhighestX == prevhighestY)
	{
		printf("Het is een vierkant.");
	}
	else if(area >= 0.17 && area <= 0.58)
	{
		printf("Het is een driehoek.");
	}
	else if(area >= 0.7 && area <= 0.86)
	{
		printf("Het is een cirkel.");
	}
	else if(area >= 0.92 && area <= 1)
	{
		printf("Dit object is niet bekend.");
	}
	else
	{
		printf("Het is een rechthoek.");
	}
}
/*
int main()
{	
	int opp;

	for(int i = 0; i <= width*height; i++)
	{
		if(crossedge(i, sobel))
		{
			//doe die ding kompiler!!
			opp++;
			printf("Er is een puntje binnen een polygoon gevonden. Counter is %i\n", opp);
		}
	}

	printf("\n Opp = %i\n", opp);

	extent(opp, sobel);

	return 0;
}
*/

std::vector<Imageprocessor::sobel> Imageprocessor::nonmax_suppression(std::vector<Imageprocessor::sobel> pixels)
{
	std::vector<Imageprocessor::sobel> ret(width * height);


	for(unsigned int index = 0; index < (width * height); index++)
	{
		if(index < width)                                                   /* Exception for the first row*/    
        {
            struct sobel s = pixels.at(index);
            ret.insert(ret.begin() + index, s);

            continue;
        }

        if(index > (height - 1) * width)                                    /* Exception for the last row */
        {
        	struct sobel s = pixels.at(index);
            ret.insert(ret.begin() + index, s);
 
	       continue;
        }

        if(!(index % width))                                                /* Exception for the first column */
        {
            struct sobel s = pixels.at(index);
            ret.insert(ret.begin() + index, s);

            continue;
        }

        if(!((index + 1) % width))                                          /* Exception for the last column */
        {
            struct sobel s = pixels.at(index);
            ret.insert(ret.begin() + index, s);

            continue;
        }


																			/* 0 degrees*/
		if((pixels[index].a >= 337.5 || pixels[index].a < 22.5) || (pixels[index].a >= 157.5 && pixels[index].a < 202.5))
		{																	/* If we found the strongest edge */
			if(pixels[index].m >= pixels[index + 1].m && pixels[index].m >= pixels[index - 1].m)
			{
				//pixels[index - 1].m = 0;
				//pixels[index + 1].m = 0;
				struct sobel s1 = pixels.at(index - 1);
				s1.m = 0;

				struct sobel s2 = pixels.at(index + 1);
				s2.m = 0;

				ret.insert(ret.begin() + index - 1, s1);
				ret.insert(ret.begin() + index + 1, s2);
			}
			continue;

		}
																			/* 45 degrees*/
		if((pixels[index].m >= 22.5 && pixels[index].m < 67.5) || (pixels[index].m >= 202.5 && pixels[index].m < 247.5))
		{																	/* If we found the strongest edge */
			if(pixels[index].m >= pixels[(index - width) + 1].m && pixels[index].m >= pixels[(index + width) - 1].m)
			{
				//pixels[(index - width) + 1].m = 0;
				//pixels[(index + width) - 1].m = 0;

				struct sobel s1 = pixels.at((index - width) + 1);
				s1.m = 0;

				struct sobel s2 = pixels.at((index + width) - 1);
				s2.m = 0;

				ret.insert(ret.begin() + ((index - width) + 1), s1);
				ret.insert(ret.begin() + ((index + width) - 1), s2);
			}
			continue;
		}
																			/* 90 degrees */
		if((pixels[index].m >= 67.5 && pixels[index].m <= 112.5) || (pixels[index].m >= 247 && pixels[index].m < 292.5))
		{																	/* If we found the strongest edge*/
			if(pixels[index].m >= pixels[index - width].m && pixels[index].m >= pixels[index + width].m)
			{
				//pixels[index - width].m = 0;
				//pixels[index + width].m = 0;

				struct sobel s1 = pixels.at(index - width);
				s1.m = 0;

				struct sobel s2 = pixels.at(index + width);
				s2.m = 0;

				ret.insert(ret.begin() + (index - width), s1);
				ret.insert(ret.begin() + (index + width), s2);
			}
			continue;
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


std::vector<Imageprocessor::sobel> Imageprocessor::fillArea(std::vector<Imageprocessor::sobel> pixels)
{
	unsigned char first_edge = 0;
	unsigned char second_edge = 0;

	size_t x1 = 0;
	size_t x2 = 0;

	unsigned char y = 0;

	for(size_t index = 0; index < (width * height); index++)
	{
		if(index / width != y)												/* If we jumped to a new line then reset all flags*/
		{
			first_edge = 0;
			second_edge = 0;

			y = index / width;
			x1 = 0;

			continue;
		}


		if(pixels[index].m > 4 && !first_edge && !second_edge)
		{
			first_edge = 1;

			x1 = index;
			y = index / width;
			continue;
		}

		if(pixels[index].m > 4 && first_edge && !second_edge)
		{
			second_edge = 1;
			continue;
		}

		if(pixels[index].m > 4 && first_edge && second_edge)
		{
			for(size_t i = x1; i <= index; i++)
				pixels[i].m = 253;

			first_edge = 0;
			second_edge = 0;
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

    image.saveToFile("GREYimage5.png");


}

/* https://stackoverflow.com/questions/35238047/how-do-i-interpret-the-orientation-of-the-gradient-when-using-imgradient-in-matl */
/* https://stackoverflow.com/questions/19815732/what-is-gradient-orientation-and-gradient-magnitude */



