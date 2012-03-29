#ifndef _Data_
#define _Data_

#include "Array.h"

// It's a singleton class

class Data
{
	friend class Star;

	public:
		// Hard coded image size and bounds for now
		static const int ni, nj;
		static const double xMin, xMax, yMin, yMax;
		static const double xRange, yRange, range;
		static const double dx, dy; // Pixel widths
		static const std::vector<Array> xy;	// Coordinates of pixel centers
				         		// These are basically meshgrids
		static const Array& x; // Just references to the elements of xy
		static const Array& y;

	private:
		Array pixels; // Pixel intensities 
		bool loaded;

	public:
		Data();
		void load(const char* filename);
		bool isLoaded() const;
		const std::vector<double>& operator [] (int i) const; // To access values
		std::vector<double>& operator [] (int i); //Non-const version
};

#endif

