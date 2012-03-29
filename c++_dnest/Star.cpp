#include "Star.h"
#include <cassert>

using namespace std;

Star::Star()
{

}

Star::Star(double x, double y, double flux)
:x(x), y(y), flux(flux)
{

}

Array& Star::incrementImage(Array& image, const PSF& psf, double coefficient) const
{
	// Calculate the image of just this star on the data grid
	Array starImage(Data::ni, Data::nj, 0.0);
	for(int i=0; i<Data::ni; i++)
		for(int j=0; j<Data::nj; j++)
			starImage[i][j] += flux*coefficient*(psf.evaluate(Data::x[i][j] - x, Data::y[i][j] - y));

	// Increment image
	image += starImage;
	return image;
}

Array& Star::incrementImage(Array& image, const PSF& psf) const
{
	return incrementImage(image, psf, 1.0);
}

Array& Star::decrementImage(Array& image, const PSF& psf) const
{
	return incrementImage(image, psf, -1.0);
}

