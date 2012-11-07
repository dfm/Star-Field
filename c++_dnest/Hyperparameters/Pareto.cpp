#include "Pareto.h"
#include <Utils.h>
#include <RandomNumberGenerator.h>
#include <cmath>
#include <iostream>

using namespace std;
using namespace DNest3;

Pareto::Pareto()
:minLogMu(log(1E-3))
,maxLogMu(log(1E3))
,rangeLogMu(maxLogMu - minLogMu)
{
	if(!Data::get_instance().isLoaded())
		cerr<<"# Error. No data has been loaded."<<endl;

	xMin = Data::get_instance().get_xMin() - 0.1*Data::get_instance().get_xRange();
	xMax = Data::get_instance().get_xMax() + 0.1*Data::get_instance().get_xRange();
	yMin = Data::get_instance().get_yMin() - 0.1*Data::get_instance().get_yRange();
	yMax = Data::get_instance().get_yMax() + 0.1*Data::get_instance().get_yRange();
}

void Pareto::fromPrior()
{
	mu = exp(minLogMu + rangeLogMu*randn());
	alpha = 1. + 4.*randomU();
}

double Pareto::perturbMu()
{
	double logH = 0.;
	mu = log(mu);
	mu += rangeLogMu*pow(10., 1.5 - 6.*randomU())*randn();
	mu = mod(mu - minLogMu, rangeLogMu) + minLogMu;
	mu = exp(mu);
	return logH;
}

double Pareto::perturbAlpha()
{
	double logH = 0.;
	alpha += 4.*pow(10., 1.5 - 6.*randomU())*randn();
	alpha = mod(alpha - 1., 4.) + 1.;
	return logH;
}

double Pareto::perturb1(vector<Star>& stars)
{
	double logH = 0.;

	for(size_t i=0; i<stars.size(); i++)
		stars[i].flux = fluxCDF(stars[i].flux);

	int which = randInt(2);
	if(which == 0)
		logH += perturbMu();
	else if(which == 1)
		logH += perturbAlpha();

	for(size_t i=0; i<stars.size(); i++)
		stars[i].flux = fluxInvCDF(stars[i].flux);
	
	return logH;
}

double Pareto::perturb2(const vector<Star>& stars)
{
	double logH = 0.;

	for(size_t i=0; i<stars.size(); i++)
		logH -= fluxLogPDF(stars[i].flux);

	int which = randInt(2);
	if(which == 0)
		logH += perturbMu();
	else if(which == 1)
		logH += perturbAlpha();

	for(size_t i=0; i<stars.size(); i++)
		logH += fluxLogPDF(stars[i].flux);
	
	return logH;
}

void Pareto::print(ostream& out) const
{
	out<<mu<<' '<<alpha;
}

Star Pareto::generateStar() const
{
	double x = xMin + (xMax - xMin)*randomU();
	double y = yMin + (yMax - yMin)*randomU();
	double f = fluxInvCDF(randomU());
	return Star(x, y, f);
}

double Pareto::perturbStar1(Star& star, double scale) const
{
	double logH = 0.;
	star.x += (xMax - xMin)*scale*randn();
	star.y += (yMax - yMin)*scale*randn();
	star.x = mod(star.x - xMin, xMax - xMin) + xMin;
	star.y = mod(star.y - yMin, yMax - yMin) + yMin;
	return logH;
}

double Pareto::perturbStar2(Star& star, double scale) const
{
	double logH = 0.;

	double u = fluxCDF(star.flux);
	u += scale*randn();
	u = mod(u, 1.);
	star.flux = fluxInvCDF(u);

	return logH;
}

double Pareto::fluxCDF(double f) const
{
	return 1. - pow(mu/f, alpha);
}

double Pareto::fluxInvCDF(double u) const
{
	return mu*pow(1. - u, -1./alpha);
}

double Pareto::fluxLogPDF(double f) const
{
	if(f < mu)
		return -1E300;
	return log(alpha) + alpha*log(mu) - (alpha + 1.)*log(f);
}

