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

