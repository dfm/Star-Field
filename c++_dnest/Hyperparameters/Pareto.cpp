#include "Pareto.h"
#include <Utils.h>
#include <RandomNumberGenerator.h>
#include <cmath>

using namespace std;
using namespace DNest3;

const double Pareto::logMinOnFraction = log(1E-3);
const double Pareto::logMaxOnFraction = log(1.);
const double Pareto::logMinFMin = log(1E-3);
const double Pareto::logMaxFMin = log(1E3);

void Pareto::fromPrior()
{
	onFraction = exp(logMinOnFraction +
			(logMaxOnFraction - logMinOnFraction)*randomU());
	fMin = exp(logMinFMin + (logMaxFMin - logMinFMin)*randomU());
	alpha = 1. + 4*randomU();
}

double Pareto::perturb()
{
	int which = randInt(3);

	if(which == 0)
	{
		fMin = log(fMin);
		fMin += (logMaxFMin - logMinFMin)
			*pow(10., 1.5 - 6.*randomU())*randn();
		fMin = mod(fMin - logMinFMin, logMaxFMin - logMinFMin) + logMinFMin;
		fMin = exp(fMin);
	}
	else if(which == 1)
	{
		onFraction = log(onFraction);
		onFraction += (logMaxOnFraction - logMinOnFraction)
				*pow(10., 1.5 - 6*randomU())*randn();
		onFraction = mod(onFraction - logMinOnFraction,
			logMaxOnFraction - logMinOnFraction) + logMinOnFraction;
		onFraction = exp(onFraction);
	}
	else
	{
		alpha += 4*pow(10., 1.5 - 6.*randomU())*randn();
		alpha = mod(alpha - 1., 4.) + 1.;
	}

	return 0.;
}

void Pareto::transform(double u_x, double u_y, double u_f,
				double& x, double& y, double& f) const
{
	double t = 1. - onFraction;
	// Compute flux
	if(u_f < t)
		f = 0.;
	else
	{
		double U = (u_f - t)/onFraction; // U(0, 1)
		f = fMin*pow(1. - U, -1./alpha); // Must use CDF not 1-CDF
	}

	// Compute position
	x = (Data::get_data().get_xMin() - 0.1*Data::get_data().get_xRange())
		 + 1.2*Data::get_data().get_xRange()*u_x;
	y = (Data::get_data().get_yMin() - 0.1*Data::get_data().get_yRange())
		 + 1.2*Data::get_data().get_yRange()*u_y;
}

void Pareto::print(ostream& out) const
{
	out<<onFraction<<' '<<fMin<<' '<<alpha;
}

