#ifndef _Star_
#define _Star_

#include "Array.h"
#include "PSF.h"
#include "Data.h"

class Star
{
	public:
		double x, y, flux;

	public:
		Star();
		Star(double x, double y, double flux);

		// Make an image of the star on the same shape and bounds as the given data
		// and add it to the input image. Returns the input image again by reference just for niceness
		Array& incrementImage(Array& image, const PSF& psf) const; // Default to coefficient=1
		Array& decrementImage(Array& image, const PSF& psf) const; // Default to coefficient=-1

		// Allows for subtraction, scaling, etc via the coefficient
		Array& incrementImage(Array& image, const PSF& psf, double coefficient) const; 
};

#endif

