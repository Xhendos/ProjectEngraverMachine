#include <vector>


class Imageprocessor
{
	private:
		void *shared_mem;										/* Pointer to the shared memory object where raw RGB values sit */
		std::vector< std::vector<int> > vec;
		unsigned int width;                                     /* Amount of pixels in the width */
		unsigned int height;                                    /* Amount of pixels in the height */

        

	public:
              
        struct sobel
        {
            short int gx;                                       /* Gradient in the x direction */
            short int gy;                                       /* Gradient in the y direction */
            short int m;                                        /* Magnitude (edge strength) */
            double    a;                                        /* Local edge orientation angle (degrees) */
        };
    	
        Imageprocessor(void *sm, unsigned int w, unsigned int h);

		std::vector<unsigned char> toGrey(void *start);
		std::vector<unsigned char> blur(std::vector<unsigned char> grey);

        std::vector<sobel> toSobel(std::vector<unsigned char> grey);    
 
};
