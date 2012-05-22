#include "Uniform.h"
#include <Utils.h>
#include <RandomNumberGenerator.h>
#include <cmath>
#include <iostream>

using namespace std;
using namespace DNest3;

Uniform::Uniform()
:minLogMu(log(1E-3))
,maxLogMu(log(1E3))
,rangeLogMu(maxLogMu - minLogMu)
{
	if(!Data::get_data().isLoaded())
		cerr<<"# Error. No data has been loaded."<<endl;

	xMin = Data::get_data().get_xMin() - 0.1*Data::get_data().get_xRange();
	xMax = Data::get_data().get_xMax() + 0.1*Data::get_data().get_xRange();
	yMin = Data::get_data().get_yMin() - 0.1*Data::get_data().get_yRange();
	yMax = Data::get_data().get_yMax() + 0.1*Data::get_data().get_yRange();
}

void Uniform::fromPrior()
{
	mu = exp(minLogMu + rangeLogMu*randn());
}

double Uniform::perturb1(vector<Star>& stars)
{
	double logH = 0.;
	double proposal = log(mu);
	proposal += rangeLogMu*pow(10., 1.5 - 6.*randomU())*randn();
	proposal = mod(proposal - minLogMu, rangeLogMu) + minLogMu;
	proposal = exp(proposal);

	double ratio = proposal/mu;
	for(size_t i=0; i<stars.size(); i++)
		stars[i].flux *= ratio;
	mu = proposal;

	return logH;
}

double Uniform::perturb2(const vector<Star>& stars)
{
	double logH = 0.;
	double proposal = log(mu);
	proposal += rangeLogMu*pow(10., 1.5 - 6.*randomU())*randn();
	proposal = mod(proposal - minLogMu, rangeLogMu) + minLogMu;
	proposal = exp(proposal);

	double logMu1 = log(mu);
	double logMu2 = log(proposal);
	for(size_t i=0; i<stars.size(); i++)
	{
		if(stars[i].flux < 0.)
			cerr<<"# Warning: Negative flux star."<<endl;
		logH -= -logMu1 - stars[i].flux/mu;
		logH += -logMu2 - stars[i].flux/proposal;
	}
	mu = proposal;
	return logH;
}

void Uniform::print(ostream& out) const
{
	out<<mu;
}

Star Uniform::generateStar() const
{
	double x = xMin + (xMax - xMin)*randomU();
	double y = yMin + (yMax - yMin)*randomU();
	double f = -mu*log(randomU());
	return Star(x, y, f);
}

double Uniform::perturbStar1(Star& star, double scale) const
{
	double logH = 0.;

	star.x += (xMax - xMin)*scale*randn();
	star.y += (yMax - yMin)*scale*randn();
	star.x = mod(star.x - xMin, xMax - xMin) + xMin;
	star.y = mod(star.y - yMin, yMax - yMin) + yMin;

	return logH;
}

double Uniform::perturbStar2(Star& star, double scale) const
{
	double logH = 0.;

	double u = fluxCDF(star.flux);
	u += scale*randn();
	u = mod(u, 1.);
	star.flux = fluxInvCDF(u);

	return logH;
}

double Uniform::fluxCDF(double f) const
{
	return 1. - exp(-f/mu);
}

double Uniform::fluxInvCDF(double u) const
{
	return -mu*log(1. - u);
}

