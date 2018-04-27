#include <stdio.h>
#include "/home/xhendos/Downloads/SFML-2.4.2/include/SFML/Graphics.hpp"

void rgb();
void grey();

int main() 
{
	char input[2];													/* Hold answer from user via command line */

	printf("RGB to picture or grey to picture? [r/g] ");
	scanf("%s", input);												/* Get a single character and store it in input[] */

	printf("Input is %s\n", input);
	

	switch(*input)
	{	
		case 'r':													/* If user wants to convert rgb to picture, go to rgb() */
		case 'R':
			rgb();
			break;
		case 'g':													/* If user wants to convert grey to picture, go to grey() */
		case 'G':
			grey();
			break;
	}	

	return 0;														/* Return 0 to the operating system (operation succesfull) */
}

void rgb()
{
	std::vector<uint8_t> data;										/* Holds all data from input file so it is easy accessable */
	uint8_t r;														/* 8bit red value */
	uint8_t g;														/* 8bit green value */
	uint8_t b;														/* 8bit blue value */

	FILE *fp = fopen("rgb.txt", "r");								/* Input file is rgb.txt. Each 8 bit value should be placed on a single line */
	if(fp == NULL)													/* If file pointer cannot be opened quit the program.*/				
	{
		perror("File pointer is zero in rgb()");
		exit(1);
	}

	for(int i = 0; i < (600 * 600); i++)							/* Image is static 600 x 600. This should be programmed flexible */
	{
		fscanf(fp, "%d", &r);										/* Read the red value and store it in int r; */
		fscanf(fp, "%d", &g);										/* Read the green value and store it in int g; */
		fscanf(fp, "%d", &b);										/* Read the blue value and store it in int b; */

		data.push_back(r);
		data.push_back(g);
		data.push_back(b);
	}
	

	sf::Image image;
	image.create(600, 600);											/* Create an 600 x 600 image pixel. This should be programmed flexible */
	for(int y = 0; y < 600; y++)									
	{																	
		for(int x = 0; x < 600; x++)
		{
			int index = (y * 600 + x) * 3;
			sf::Color c(data[index], data[index+1], data[index+2]);
			image.setPixel(x, y, c);
		}
	}	
	image.saveToFile("RGBpicture.png");

}

void grey()
{
	std::vector<uint8_t>data;	
	
	FILE *fp = fopen("grey.txt", "r");
	if(fp == NULL) printf("Error opening new.txt\n");

	int byte;
	for(int i = 0; i < (600*600); i++)
	{
		fscanf(fp, "%d", &byte);
		data.push_back(byte);
	}
	
 
    sf::Image image;
    image.create(600, 600);
    for(int y=0; y<600; y++){
        for(int x=0; x<600; x++){
            int index=(y*600 + x);
            sf::Color c(data[index], data[index], data[index]);
            image.setPixel(x,y,c);
        }
    }
    image.saveToFile("GREYpicture.png");
}
