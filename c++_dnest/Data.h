#ifndef _Data_
#define _Data_

#include "Image.h"

class Data
{
	private:
		Image image; // Pixel intensities

	public:
		Data();
		void load(const char* filename);

		// Access to pixel values
		double& operator () (int i, int j);
};

#endif

