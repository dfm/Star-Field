#include "Star.h"
#include "Data.h"
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
	for(int i=0; i<Data::get_data().get_ni(); i++)
	{
		for(int j=0; j<Data::get_data().get_nj(); j++)
		{
			image(i, j) += coefficient*flux*
	(psf.evaluate(Data::get_data().get_xc(i, j) - x, Data::get_data().get_yc(i, j) - y));
		}
	}

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

