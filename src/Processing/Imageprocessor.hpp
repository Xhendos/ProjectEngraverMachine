#include <vector>

class Imageprocessor
{
	private:
		void *shared_mem;										/* Pointer to the shared memory object where raw RGB values sit */
		std::vector< std::vector<int> > vec;					
	
		int width;
		int height;
		
	public:
		Imageprocessor(void *sm, int w, int h);
		
		std::vector<unsigned char> toGrey(void *start);
};
