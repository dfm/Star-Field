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

		int ni, nj;
		double xMin, xMax, yMin, yMax;
		double xRange, yRange, dx, dy, dA;

		// Pixel centers
		Array xc, yc;

	public:
		Data();
		void load(const char* filename);
		bool isLoaded() const { return loaded; }

		// Various getters
		int get_ni() const { return ni; }
		int get_nj() const { return nj; }
		double get_xMin() const { return xMin; }
		double get_xMax() const { return xMax; }
		double get_yMin() const { return yMin; }
		double get_yMax() const { return yMax; }
		double get_xRange() const { return xRange; }
		double get_yRange() const { return yRange; }
		double get_dx() const { return dx; }
		double get_dy() const { return dy; }
		double get_dA() const { return dA; }

		// Getters for xc and yc
		double get_xc(int i, int j) const { return xc(i, j); }
		double get_yc(int i, int j) const { return yc(i, j); }

		// Getter for pixel values
		double operator () (int i, int j) const { return image(i, j); }


	// Static stuff for single global instance
	private:
		static Data instance;

	public:
		// Getter and loader
		static const Data& get_instance() { return instance; } 
		static void load_data(const char* filename) { instance.load(filename); }
};

#endif

