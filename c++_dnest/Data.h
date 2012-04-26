#ifndef _Data_
#define _Data_

#include <vector>
#include "Image.h"
#include "Array.h"

/*
* An instance of this class represents the data set.
* Private constructor --> it's a singleton
*/

class Data
{
	private:
		Image image; // Pixel intensities
		bool loaded;

		// Pixel centers
		Array xc, yc;

	public:
		Data();
		void load(const char* filename);

		// Read-only getter for the image
		const Image& get_image() const { return image; }

		// Getters for xc and yc
		double get_xc(int i, int j) const { return xc(i, j); }
		double get_yc(int i, int j) const { return yc(i, j); }
};

#endif

