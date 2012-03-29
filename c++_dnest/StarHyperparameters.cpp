#include "StarHyperparameters.h"
#include "Data.h"
#include <RandomNumberGenerator.h>
#include <Utils.h>
#include <cmath>
#include <iostream>

using namespace DNest;
using namespace std;

const double StarHyperparameters::minMeanFlux = 1E-3;
const double StarHyperparameters::maxMeanFlux = 1E3;

StarHyperparameters::StarHyperparameters()
{

}

void StarHyperparameters::fromPrior(int thread)
{
	xMean = Data::xMin + Data::xRange*randomU(thread);
	yMean = Data::yMin + Data::yRange*randomU(thread);
	sig = exp(log(1E-2*Data::range) + log(1E2)*randomU(thread));
	meanFlux = exp(log(minMeanFlux) + log(maxMeanFlux/minMeanFlux)*randomU(thread));
}

Star StarHyperparameters::generateStar(int thread) const
{
	double x, y, flux;
	x = xMean + sig*randn(thread);
	y = yMean + sig*randn(thread);
	flux = -meanFlux*log(randomU(thread));
	return Star(x, y, flux);
}

double StarHyperparameters::logp(const Star& star) const
{
	double result = 0.0;
	double var = sig*sig;
	result += -log(2*M_PI*var) - 0.5*(pow(star.x - xMean, 2) + pow(star.y - yMean, 2))/var;
	result += -log(meanFlux) - star.flux/meanFlux;
	if(star.flux < 0)
		result = -1E300;
	return result;
}

double StarHyperparameters::logp(const vector<Star>& stars) const
{
	double result = 0.0;
	for(size_t i=0; i<stars.size(); i++)
		result += logp(stars[i]);
	return result;
}

double StarHyperparameters::perturb(int thread)
{
	double scale = pow(10.0, 1.5 - 6*randomU(thread));

	int which = randInt(thread, 3);
	if(which == 0)
	{
		xMean += Data::xRange*scale*randn(thread);
		yMean += Data::yRange*scale*randn(thread);
		xMean = mod(xMean - Data::xMin, Data::xRange) + Data::xMin;
		yMean = mod(yMean - Data::yMin, Data::yRange) + Data::yMin;
	}
	else if(which == 1)
	{
		sig = log(sig);
		sig += log(1E2)*scale*randn(thread);
		sig = mod(sig - log(1E-2*Data::range), log(1E2)) + log(1E-2*Data::range);
		sig = exp(sig);
	}
	else if(which == 2)
	{
		meanFlux = log(meanFlux);
		meanFlux += log(maxMeanFlux/minMeanFlux)*scale*randn(thread);
		meanFlux = mod(meanFlux - log(minMeanFlux), log(maxMeanFlux/minMeanFlux)) + log(minMeanFlux);
		meanFlux = exp(meanFlux);
	}

	return 0;
}

double StarHyperparameters::perturb(int thread, vector<Star>& stars)
{
	double scale = pow(10.0, 1.5 - 6*randomU(thread));

	int which = randInt(thread, 3);
	if(which == 0)
	{
		double xDiff = -xMean;
		double yDiff = -yMean;
		xMean += Data::xRange*scale*randn(thread);
		yMean += Data::yRange*scale*randn(thread);
		xMean = mod(xMean - Data::xMin, Data::xRange) + Data::xMin;
		yMean = mod(yMean - Data::yMin, Data::yRange) + Data::yMin;
		xDiff += xMean;
		yDiff += yMean;
		for(size_t i=0; i<stars.size(); i++)
		{
			stars[i].x += xDiff;
			stars[i].y += yDiff;	
		}
	}
	else if(which == 1)
	{
		double ratio = 1.0/meanFlux;
		meanFlux = log(meanFlux);
		meanFlux += log(maxMeanFlux/minMeanFlux)*scale*randn(thread);
		meanFlux = mod(meanFlux - log(minMeanFlux), log(maxMeanFlux/minMeanFlux)) + log(minMeanFlux);
		meanFlux = exp(meanFlux);
		ratio *= meanFlux;
		for(size_t i=0; i<stars.size(); i++)
			stars[i].flux *= ratio;
	}
	else if(which == 2)
	{
		double ratio = 1.0/sig;
		sig = log(sig);
		sig += log(1E2)*scale*randn(thread);
		sig = mod(sig - log(1E-2*Data::range), log(1E2)) + log(1E-2*Data::range);
		sig = exp(sig);
		ratio *= sig;

		for(size_t i=0; i<stars.size(); i++)
		{
			stars[i].x = ratio*(stars[i].x - xMean) + xMean;
			stars[i].y = ratio*(stars[i].y - yMean) + yMean;
		}
	}

	return 0;
}

ostream& operator << (ostream& out, const StarHyperparameters& hyp)
{
	out<<hyp.xMean<<' '<<hyp.yMean<<' '<<hyp.sig<<' '<<hyp.meanFlux;
	return out;
}

