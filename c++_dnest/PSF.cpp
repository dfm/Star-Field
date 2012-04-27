#include "PSF.h"
#include "Star.h"
#include <cmath>
#include <cassert>
#include <iostream>

using namespace std;

const double PSF::edge = 5.0;

PSF::PSF(double sigma1, double sigma2, double weight)
{
	set(sigma1, sigma2, weight);
}

double PSF::evaluate(double x, double y) const
{
	double rsq = x*x + y*y;
	double result = 0.;
	if(rsq < rsqEdge)
	{
		result = weight*preFactor1*exp(-0.5*rsq/(sigma1*sigma1))
			+ (1.0 - weight)*preFactor2*exp(-0.5*rsq/(sigma2*sigma2));
	}
	return result;
}

void PSF::set(double sigma1, double sigma2, double weight)
{
	assert(weight >= 0 && weight <= 1);
	assert(sigma1 < sigma2);

	this->sigma1 = sigma1;
	this->sigma2 = sigma2;
	this->weight = weight;
	preFactor1 = 1./(sigma1*sigma1)/(2*M_PI);
	preFactor2 = 1./(sigma2*sigma2)/(2*M_PI);
	rsqEdge = pow(PSF::edge*sigma2, 2);
}

