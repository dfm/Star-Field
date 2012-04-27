#include "NYU.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include "../Data.h"
#include <cmath>

using namespace std;
using namespace DNest;

const double NYU::minMu = 1E-3;
const double NYU::maxMu = 1E3;
const double NYU::minSigma = 1E-2*sqrt(Data::get_data().get_xRange()*Data::get_data().get_yRange());
const double NYU::maxSigma = 100*NYU::minSigma;

void NYU::fromPrior()
{
	xc = Data::get_data().get_xMin() + Data::get_data().get_xRange()*randomU();
	yc = Data::get_data().get_yMin() + Data::get_data().get_yRange()*randomU();
	sigma = exp(log(minSigma) + log(maxSigma/minSigma)*randomU());
	mu = exp(log(minMu) + log(maxMu/minMu)*randomU());
}

double NYU::perturb()
{
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
	return 0.;
}

double NYU::perturb(std::vector<Star>& stars)
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

double NYU::perturbStar(Star& star) const
{
	double scale = pow(10., 1.5 - 6.*randomU());
	if(randomU() <= 0.5)
	{
		star.x += sigma*scale*randn();
		star.y += sigma*scale*randn();
	}
	else
	{
		star.flux = 1. - exp(-star.flux/mu);
		star.flux += scale*randn();
		
	}
	return 0.;
}

// Evaluate the probability density
// for a star given the hyperparameters
double NYU::_logp(const Star& star) const
{
	double result = 0.;
	if(star.x < Data::get_data().get_xMin() || star.x > Data::get_data().get_xMax()
		|| star.y < Data::get_data().get_yMin() || star.y > Data::get_data().get_yMax())
		result = -1E300;
	return result;
}

ostream& operator << (ostream& out, const NYU& i)
{
	return out;
}

