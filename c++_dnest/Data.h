#ifndef _Data_
#define _Data_

#include "Array.h"

// It's a singleton class

class Data
{
	private:
		// Hard coded for now
		static const int numPixels;

		Array pixels; // Pixel intensities

	public:
		Data();
		void load(const char* filename);

		// Read-only access to values
		double operator () (int i, int j) const;
};

#endif

