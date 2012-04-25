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

		static Data instance; // Singleton instance
		Data();

	public:
		static void load(const char* filename);

		// Read-only getter for the image
		static const Image& get_image() { return instance.image; }
};

#endif

