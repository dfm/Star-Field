#include "PSF.h"
#include "Star.h"
#include "Lookup.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include <cmath>
#include <cassert>
#include <iostream>

using namespace std;
using namespace DNest3;

const double PSF::edge = 5.0;

PSF::PSF()
{

}

PSF::PSF(double sigma1, double sigma2, double weight)
:sigma1(sigma1), sigma2(sigma2), weight(weight)
{
	sync();
}

double PSF::evaluate(double x, double y) const
{
	double rsq = x*x + y*y;
	double result = 0.;
	if(rsq < rsqEdge)
	{
		result = weight*preFactor1*Lookup::evaluate(0.5*rsq*pr1)
			+ (1.0 - weight)*preFactor2*Lookup::evaluate(0.5*rsq*pr2);
	}
	return result;
}

void PSF::sync()
{
	pr1 = pow(sigma1, -2);
	pr2 = pow(sigma2, -2);
	preFactor1 = pr1/(2*M_PI);
	preFactor2 = pr2/(2*M_PI);
	rEdge = PSF::edge*sigma2;
	rsqEdge = pow(rEdge, 2);
}

void PSF::fromPrior()
{
	// Log-uniform between 0.3 and 30 pixel widths
	sigma1 = exp(log(0.3*Data::get_instance().get_dx()) + log(100.)*randomU());
	double diff = 2.3*randomU();
	sigma2 = exp(log(sigma1) + diff);
	weight = randomU();

	sync();
}

double PSF::perturb()
{
	double logH = 0.;

	int which = randInt(3);
	if(which == 0)
	{
		double diff = log(sigma2) - log(sigma1);

		sigma1 = log(sigma1);
		sigma1 += log(100.)*pow(10., 1.5 - 6.*randomU())*randn();
		sigma1 = mod(sigma1 - log(0.3*Data::get_instance().get_dx()),
				log(100.)) + log(0.3*Data::get_instance().get_dx());
		sigma1 = exp(sigma1);

		sigma2 = exp(log(sigma1) + diff);
	}
	else if(which == 1)
	{
		double diff = log(sigma2) - log(sigma1);
		diff += 2.3*pow(10., 1.5 - 6.*randomU())*randn();
		diff = mod(diff, 2.3);
		sigma2 = exp(log(sigma1) + diff);
	}
	else
	{
		weight += pow(10., 1.5 - 6.*randomU())*randn();
		weight = mod(weight, 1.);
	}

	sync();
	return logH;
}

ostream& operator << (ostream& out, const PSF& psf)
{
	out<<psf.sigma1<<' '<<psf.sigma2<<' '<<psf.weight;
	return out;
}

