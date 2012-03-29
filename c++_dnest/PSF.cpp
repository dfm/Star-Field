#include "PSF.h"
#include "Data.h"
#include "Star.h"
#include <cmath>
#include <cassert>
#include <iostream>

using namespace std;

const double PSF::edge = 5.0;

PSF::PSF(double sigma1, double sigma2, double weight)
:sigma1(sigma1), sigma2(sigma2), weight(weight)
,prefactor1(1.0/(sigma1*sigma1*2*M_PI))
,prefactor2(1.0/(sigma2*sigma2*2*M_PI))
,norm(1.0)
{
	assert(weight >= 0 && weight <= 1);
	assert(sigma1 < sigma2);

	// Calculate normalisation by putting a star in the middle of the data field
	Array image(Data::ni, Data::nj, 0.0);
	Star s((Data::xMin + Data::xMax)/2, (Data::yMin + Data::yMax)/2, 1.0);
	s.incrementImage(image, *this);
	norm = 1.0/image.sum();
}

double PSF::evaluate(double x, double y) const
{
	double rsq = x*x + y*y;
	return evaluate(rsq);
}

double PSF::evaluate(double rsq) const
{
	double result;
	if(rsq < edge*sigma2*sigma2)
	{
		result = weight*prefactor1*exp(-0.5*rsq/(sigma1*sigma1)) + (1.0 - weight)*prefactor2*exp(-0.5*rsq/(sigma2*sigma2));
		result *= norm;
	}
	else
		result = 0.0;
	return result;
}


Array PSF::evaluate(const Array& x, const Array& y) const
{
	Array rsq = x;
	for(int i=0; i<rsq.getNi(); i++)
		for(int j=0; j<rsq.getNj(); j++)
			rsq[i][j] = x[i][j]*x[i][j] + y[i][j]*y[i][j];
	return evaluate(rsq);
}

Array PSF::evaluate(const Array& rsq) const
{
	Array result = rsq;
	for(int i=0; i<rsq.getNi(); i++)
		for(int j=0; j<rsq.getNj(); j++)
			result[i][j] = evaluate(rsq[i][j]);
	return result;
}

