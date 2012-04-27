#include "Star.h"
#include <cassert>
#define PSF_NORMALISE

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
	Array starImage(Data::get_data().get_ni(), Data::get_data().get_nj());
	starImage.setZero();

	for(int i=0; i<Data::get_data().get_ni(); i++)
	{
		for(int j=0; j<Data::get_data().get_nj(); j++)
		{
			starImage(i, j) += flux*coefficient*
	(psf.evaluate(Data::get_data().get_xc(i, j) - x, Data::get_data().get_yc(i, j) - y));
		}
	}

	#ifdef PSF_NORMALISE
		double sum = starImage.sum();
		if(sum != 0)
			starImage *= flux/sum;
	#endif

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

