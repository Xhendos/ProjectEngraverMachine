#include <vector>


class Imageprocessor
{
	private:
		void *shared_mem;										/* Pointer to the shared memory object where raw RGB values sit */
		std::vector< std::vector<int> > vec;
		unsigned int width;
		unsigned int height;



    struct sobel
    {
        short int gx;
        short int gy;
        short int m;
    };


	public:
    	Imageprocessor(void *sm, unsigned int w, unsigned int h);

		std::vector<unsigned char> toGrey(void *start);
		std::vector<unsigned char> blur(std::vector<unsigned char> grey);

        std::vector<struct sobel> sobelOperator(std::vector<unsigned char> grey);    
    
};
