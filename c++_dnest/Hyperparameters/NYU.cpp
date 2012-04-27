#include "NYU.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include "../Data.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace DNest;

double NYU::minMu = 1E-6;
double NYU::maxMu = 1;
double NYU::minSigma;
double NYU::maxSigma;

void NYU::fromPrior()
{
	if(!Data::get_data().isLoaded())
		cerr<<"WARNING: Data not loaded."<<endl;

	maxSigma = sqrt(Data::get_data().get_xRange()*Data::get_data().get_yRange());
	minSigma = 1E-2*maxSigma;

	xc = Data::get_data().get_xMin() + Data::get_data().get_xRange()*randomU();
	yc = Data::get_data().get_yMin() + Data::get_data().get_yRange()*randomU();
	sigma = exp(log(minSigma) + log(maxSigma/minSigma)*randomU());
	mu = exp(log(minMu) + log(maxMu/minMu)*randomU());
}

double NYU::perturb1(const vector<Star>& stars)
{
	double logH = -logp(stars);
	int which = randInt(3);

	double scale = pow(10., 1.5 - 6.*randomU());
	if(which == 0)
	{
		xc += Data::get_data().get_xRange()*scale*randn();
		yc += Data::get_data().get_yRange()*scale*randn();
		xc = mod(xc - Data::get_data().get_xMin(), Data::get_data().get_xRange())
			+ Data::get_data().get_xMin();
		yc = mod(yc - Data::get_data().get_yMin(), Data::get_data().get_yRange())
			+ Data::get_data().get_yMin();
	}
	else if(which == 1)
	{
		sigma = log(sigma);
		sigma += log(maxSigma/minSigma)*scale*randn();
		sigma = mod(sigma - log(minSigma), log(maxSigma/minSigma)) + log(minSigma);
		sigma = exp(sigma);
	}
	else
	{
		mu = log(mu);
		mu += log(maxMu/minMu)*scale*randn();
		mu = mod(mu - log(minMu), log(maxMu/minMu)) + log(minMu);
		mu = exp(mu);
	}

	logH += logp(stars);
	return logH;
}

double NYU::perturb2(std::vector<Star>& stars)
{
	int which = randInt(3);

	double scale = pow(10., 1.5 - 6.*randomU());
	if(which == 0)
	{
		double diff_x = -xc;
		double diff_y = -yc;
		xc += Data::get_data().get_xRange()*scale*randn();
		yc += Data::get_data().get_yRange()*scale*randn();
		xc = mod(xc - Data::get_data().get_xMin(), Data::get_data().get_xRange())
			+ Data::get_data().get_xMin();
		yc = mod(yc - Data::get_data().get_yMin(), Data::get_data().get_yRange())
			+ Data::get_data().get_yMin();
		diff_x += xc;
		diff_y += yc;

		for(size_t i=0; i<stars.size(); i++)
		{
			stars[i].x += diff_x;
			stars[i].y += diff_y;
		}
	}
	else if(which == 1)
	{
		double ratio = 1.0/sigma;
		sigma = log(sigma);
		sigma += log(maxSigma/minSigma)*scale*randn();
		sigma = mod(sigma - log(minSigma), log(maxSigma/minSigma)) + log(minSigma);
		sigma = exp(sigma);
		ratio *= sigma;
		for(size_t i=0; i<stars.size(); i++)
		{
			stars[i].x = ratio*(stars[i].x - xc) + xc;
			stars[i].y = ratio*(stars[i].y - yc) + yc;
		}
	}
	else
	{
		double ratio = 1.0/mu;
		mu = log(mu);
		mu += log(maxMu/minMu)*scale*randn();
		mu = mod(mu - log(minMu), log(maxMu/minMu)) + log(minMu);
		mu = exp(mu);
		ratio *= mu;
		for(size_t i=0; i<stars.size(); i++)
		{
			stars[i].flux *= ratio;
		}
	}
	return 0.;
}

// Generate a star from the prior
// given the hyperparameters
Star NYU::generateStar() const
{
	double x = xc + sigma*randn();
	double y = yc + sigma*randn();
	double flux = -mu*log(randomU());
	return Star(x, y, flux);
}

double NYU::perturbPosition(Star& star, double scale) const
{
	double logH = -_logp(star);
	star.x += sigma*scale*randn();
	star.y += sigma*scale*randn();
	logH += _logp(star);
	return 0.;
}

double NYU::perturbFlux(Star& star, double scale) const
{
	star.flux = 1. - exp(-star.flux/mu);
	star.flux += scale*randn();
	star.flux = mod(star.flux, 1.0);
	star.flux = -mu*log(1. - star.flux);
	return 0.;
}

// Evaluate the probability density
// for a star given the hyperparameters
double NYU::_logp(const Star& star) const
{
	double result = 0.;
	result += -log(2*M_PI*sigma*sigma) - 0.5*pow((star.x - xc)/sigma, 2)
					   - 0.5*pow((star.y - yc)/sigma, 2);
	result += -log(mu) - star.flux/mu;
	if(star.flux < 0)
		result = -1E300;
	return result;
}

ostream& operator << (ostream& out, const NYU& i)
{
	out<<i.xc<<' '<<i.yc<<' '<<i.sigma<<' '<<i.mu;
	return out;
}

