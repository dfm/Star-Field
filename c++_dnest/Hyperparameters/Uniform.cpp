#include "Uniform.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include "../Data.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace DNest;

double Uniform::minMu = 1E-3;
double Uniform::maxMu = 1E3;

void Uniform::fromPrior()
{
	if(!Data::get_data().isLoaded())
		cerr<<"WARNING: Data not loaded."<<endl;

	double xMin = Data::get_data().get_xMin()
			- 0.1*Data::get_data().get_xRange();
	double xMax = Data::get_data().get_xMax()
			+ 0.1*Data::get_data().get_xRange();
	double yMin = Data::get_data().get_yMin()
			- 0.1*Data::get_data().get_yRange();
	double yMax = Data::get_data().get_yMax()
			+ 0.1*Data::get_data().get_yRange();

	mu = exp(log(minMu) + log(maxMu/minMu)*randomU());
}

double Uniform::perturb1(const vector<Star>& stars)
{
	double logH = -logp(stars);
	double scale = pow(10., 1.5 - 6.*randomU());

	mu = log(mu);
	mu += log(maxMu/minMu)*scale*randn();
	mu = mod(mu - log(minMu), log(maxMu/minMu)) + log(minMu);
	mu = exp(mu);

	logH += logp(stars);
	return logH;
}

double Uniform::perturb2(std::vector<Star>& stars)
{
	double scale = pow(10., 1.5 - 6.*randomU());

	double ratio = 1.0/mu;
	mu = log(mu);
	mu += log(maxMu/minMu)*scale*randn();
	mu = mod(mu - log(minMu), log(maxMu/minMu)) + log(minMu);
	mu = exp(mu);
	ratio *= mu;
	for(size_t i=0; i<stars.size(); i++)
		stars[i].flux *= ratio;

	return 0.;
}

// Generate a star from the prior
// given the hyperparameters
Star Uniform::generateStar() const
{
	double x = xMin + (xMax - xMin)*randomU();
	double y = yMin + (yMax - yMin)*randomU();
	double flux = -mu*log(randomU());
	return Star(x, y, flux);
}

double Uniform::perturbPosition(Star& star, double scale) const
{
	star.x += (xMax - xMin)*scale*randn();
	star.x = mod(star.x - xMin, xMax - xMin) + xMin;
	star.y += (yMax - yMin)*scale*randn();
	star.y = mod(star.y - yMin, yMax - yMin) + yMin;
	return 0.;
}

double Uniform::perturbFlux(Star& star, double scale) const
{
	star.flux = 1. - exp(-star.flux/mu);
	star.flux += scale*randn();
	star.flux = mod(star.flux, 1.0);
	star.flux = -mu*log(1. - star.flux);
	return 0.;
}

// Evaluate the probability density
// for a star given the hyperparameters
double Uniform::_logp(const Star& star) const
{
	return 0.;
}

ostream& operator << (ostream& out, const Uniform& i)
{
	out<<i.mu;
	return out;
}

