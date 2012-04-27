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

Array& Star::incrementImage(Array& image, const PSF& psf, const Data& data, double coefficient) const
{
	// Calculate the image of just this star on the data grid
	Array starImage(data.get_ni(), data.get_nj());
	starImage.setZero();

	for(int i=0; i<data.get_ni(); i++)
	{
		for(int j=0; j<data.get_nj(); j++)
		{
			starImage(i, j) += flux*coefficient*
	(psf.evaluate(data.get_xc(i, j) - x, data.get_yc(i, j) - y));
		}
	}

	// Increment image
	image += starImage;

	return image;
}

Array& Star::incrementImage(Array& image, const PSF& psf, const Data& data) const
{
	return incrementImage(image, psf, data, 1.0);
}

Array& Star::decrementImage(Array& image, const PSF& psf, const Data& data) const
{
	return incrementImage(image, psf, data, -1.0);
}

