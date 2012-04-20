#ifndef _Data_
#define _Data_

#include "Image.h"

// It's a singleton class

class Data
{
	private:
		Image image; // Pixel intensities

	public:
		Data();
		void load(const char* filename);

		// Read-only access to values
		double operator () (int i, int j) const;
};

#endif

