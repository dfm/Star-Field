#ifndef _Data_
#define _Data_

#include <vector>
#include "Array.h"

/*
* An instance of this class represents a data set.
*/

class Data
{
	private:
		Array image; // Pixel intensities
		bool loaded;

		double ni, nj, xMin, xMax, yMin, yMax;
		double xRange, yRange, dx, dy;

		// Pixel centers
		Array xc, yc;

	public:
		Data();
		void load(const char* filename);

		// Getters for xc and yc
		double get_xc(int i, int j) const { return xc(i, j); }
		double get_yc(int i, int j) const { return yc(i, j); }
};

#endif

