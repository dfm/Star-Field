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
	double xMin = Data::get_data().get_xMin() - 0.1*Data::get_data().get_xRange();
	double xMax = Data::get_data().get_xMax() + 0.1*Data::get_data().get_xRange();
	double yMin = Data::get_data().get_yMin() - 0.1*Data::get_data().get_yRange();
	double yMax = Data::get_data().get_yMax() + 0.1*Data::get_data().get_yRange();
	double x = xMin + (xMax - xMin)*randomU();
	double y = yMin + (yMax - yMin)*randomU();
	double f = -mu*log(randomU());
	return Star(x, y, f);
}

double Uniform::logp(const Star& star) const
{
	double xMin = Data::get_data().get_xMin() - 0.1*Data::get_data().get_xRange();
	double xMax = Data::get_data().get_xMax() + 0.1*Data::get_data().get_xRange();
	double yMin = Data::get_data().get_yMin() - 0.1*Data::get_data().get_yRange();
	double yMax = Data::get_data().get_yMax() + 0.1*Data::get_data().get_yRange();

	if(star.flux < 0. || star.x < xMin || star.x > xMax
		|| star.y < yMin || star.y > yMax)
		result = -1E300;
	else
		result = -log(mu) - star.flux/mu;
	return result;
}

