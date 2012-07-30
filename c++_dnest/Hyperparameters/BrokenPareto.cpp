#include "BrokenPareto.h"
#include <Utils.h>
#include <RandomNumberGenerator.h>
#include <cmath>
#include <iostream>

using namespace std;
using namespace DNest3;

BrokenPareto::BrokenPareto()
{
	if(!Data::get_data().isLoaded())
		cerr<<"# Error. No data has been loaded."<<endl;

	xMin = Data::get_data().get_xMin() - 0.1*Data::get_data().get_xRange();
	xMax = Data::get_data().get_xMax() + 0.1*Data::get_data().get_xRange();
	yMin = Data::get_data().get_yMin() - 0.1*Data::get_data().get_yRange();
	yMax = Data::get_data().get_yMax() + 0.1*Data::get_data().get_yRange();
}

void BrokenPareto::fromPrior()
{
	x1 = log(1E-3) + log(1E6)*randomU();
	x2 = x1 + 10.*randomU();
	x1 = exp(x1); x2 = exp(x2);

	a1 = 1. + 4*randomU();
	a2 = 1. + 4*randomU();
}

double BrokenPareto::perturb_x()
{
	double logH = 0.;

	if(randomU() <= 0.5)
	{
		x1 = log(x1); x2 = log(x2);

		double diff = log(1E6)*pow(10., 1.5 - 6.*randomU())*randn();
		x1 += diff;
		x2 += diff;

		x1 = exp(x1); x2 = log(x2);
	}
	else
	{
		x1 = log(x1); x2 = log(x2);

		double diff = x2 - x1;
		diff += 10.*pow(10., 1.5 - 6.*randomU())*randn();
		diff = mod(diff, 10.);
		x2 = x1 + diff;

		x1 = exp(x1); x2 = log(x2);	
	}

	return logH;
}

double BrokenPareto::perturbAlpha()
{
	double logH = 0.;

	if(randomU() <= 0.5)
	{
		a1 += 4.*pow(10., 1.5 - 6.*randomU())*randn();
		a1 = mod(a1 - 1., 4.) + 1.;
	}
	else
	{
		a2 += 4.*pow(10., 1.5 - 6.*randomU())*randn();
		a2 = mod(a2 - 1., 4.) + 1.;
	}
	return logH;
}

double BrokenPareto::perturb1(vector<Star>& stars)
{
	double logH = 0.;

	for(size_t i=0; i<stars.size(); i++)
		stars[i].flux = fluxCDF(stars[i].flux);

	int which = randInt(2);
	if(which == 0)
		logH += perturb_x();
	else if(which == 1)
		logH += perturb_a();

	for(size_t i=0; i<stars.size(); i++)
		stars[i].flux = fluxInvCDF(stars[i].flux);
	
	return logH;
}

double BrokenPareto::perturb2(const vector<Star>& stars)
{
	double logH = 0.;

	for(size_t i=0; i<stars.size(); i++)
		logH -= fluxLogPDF(stars[i].flux);

	int which = randInt(2);
	if(which == 0)
		logH += perturb_x();
	else if(which == 1)
		logH += perturb_a();

	for(size_t i=0; i<stars.size(); i++)
		logH += fluxLogPDF(stars[i].flux);
	
	return logH;
}

void BrokenPareto::print(ostream& out) const
{
	out<<mu<<' '<<alpha;
}

Star BrokenPareto::generateStar() const
{
	double x = xMin + (xMax - xMin)*randomU();
	double y = yMin + (yMax - yMin)*randomU();
	double f = fluxInvCDF(randomU());
	return Star(x, y, f);
}

double BrokenPareto::perturbStar1(Star& star, double scale) const
{
	double logH = 0.;
	star.x += (xMax - xMin)*scale*randn();
	star.y += (yMax - yMin)*scale*randn();
	star.x = mod(star.x - xMin, xMax - xMin) + xMin;
	star.y = mod(star.y - yMin, yMax - yMin) + yMin;
	return logH;
}

double BrokenPareto::perturbStar2(Star& star, double scale) const
{
	double logH = 0.;

	double u = fluxCDF(star.flux);
	u += scale*randn();
	u = mod(u, 1.);
	star.flux = fluxInvCDF(u);

	return logH;
}

double BrokenPareto::fluxCDF(double f) const
{
	return 1. - pow(mu/f, alpha);
}

double BrokenPareto::fluxInvCDF(double u) const
{
	return mu*pow(1. - u, -1./alpha);
}

double BrokenPareto::fluxLogPDF(double f) const
{
	if(f < mu)
		return -1E300;
	return log(alpha) + alpha*log(mu) - (alpha + 1.)*log(f);
}

