#ifndef _Data_
#define _Data_

#include "Image.h"

/*
* An instance of this class represents the data set.
* Private constructor --> it's a singleton
*/

class Data
{
	private:
		Image image; // Pixel intensities
		bool loaded;

	public:
		Data();
		void load(const char* filename);

		// Read-only getter for the image
		const Image& get_image() { return image; }
};

#endif

