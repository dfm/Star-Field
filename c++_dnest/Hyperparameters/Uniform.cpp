#include "Uniform.h"
#include <Utils.h>
#include <RandomNumberGenerator.h>
#include <cmath>

using namespace std;
using namespace DNest3;

const double Uniform::logMinOnFraction = log(1E-3);
const double Uniform::logMaxOnFraction = log(1.);
const double Uniform::logMinMu = log(1E-3);
const double Uniform::logMaxMu = log(1E3);

void Uniform::fromPrior()
{
	onFraction = exp(logMinOnFraction +
			(logMaxOnFraction - logMinOnFraction)*randomU());
	mu = exp(logMinMu + (logMaxMu - logMinMu)*randomU());
}

double Uniform::perturb()
{
	int which = randInt(2);

	if(which == 0)
	{
		mu = log(mu);
		mu += (logMaxMu - logMinMu)
			*pow(10., 1.5 - 6.*randomU())*randn();
		mu = mod(mu - logMinMu, logMaxMu - logMinMu) + logMinMu;
		mu = exp(mu);
	}
	else
	{
		onFraction = log(onFraction);
		onFraction += (logMaxOnFraction - logMinOnFraction)
				*pow(10., 1.5 - 6*randomU())*randn();
		onFraction = mod(onFraction - logMinOnFraction,
			logMaxOnFraction - logMinOnFraction) + logMinOnFraction;
		onFraction = exp(onFraction);
	}

	return 0.;
}

void Uniform::transform(double u_x, double u_y, double u_f,
				double& x, double& y, double& f) const
{
	double t = 1. - onFraction;
	// Compute flux
	if(u_f < t)
		f = 0.;
	else
	{
		double U = (u_f - t)/onFraction; // U(0, 1)
		f = -mu*log(1. - U); // Must use CDF not 1-CDF
	}

	// Compute position
	x = (Data::get_data().get_xMin() - 0.1*Data::get_data().get_xRange())
		 + 1.2*Data::get_data().get_xRange()*u_x;
	y = (Data::get_data().get_yMin() - 0.1*Data::get_data().get_yRange())
		 + 1.2*Data::get_data().get_yRange()*u_y;
}

void Uniform::print(ostream& out) const
{
	out<<onFraction<<' '<<mu;
}



