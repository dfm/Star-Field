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
	if(flux == 0.)
		return image;

	int iMin = (Data::get_instance().get_yMax() - (y + psf.get_rEdge()))/Data::get_instance().get_dy();
	int iMax = (Data::get_instance().get_yMax() - (y - psf.get_rEdge()))/Data::get_instance().get_dy();
	int jMin = (x - psf.get_rEdge() - Data::get_instance().get_xMin())/Data::get_instance().get_dx();
	int jMax = (x + psf.get_rEdge() - Data::get_instance().get_xMin())/Data::get_instance().get_dx();

	// Prevent going out of bounds
	if(iMin < 0)
		iMin = 0;
	if(iMax >= Data::get_instance().get_ni())
		iMax = Data::get_instance().get_ni() - 1;
	if(jMin < 0)
		jMin = 0;
	if(jMax >= Data::get_instance().get_nj())
		jMax = Data::get_instance().get_nj() - 1;

	for(int i=iMin; i<=iMax; i++)
	{
		for(int j=jMin; j<=jMax; j++)
		{
			image(i, j) += coefficient*flux*
	(psf.evaluate(Data::get_instance().get_xc(i, j) - x, Data::get_instance().get_yc(i, j) - y));
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

