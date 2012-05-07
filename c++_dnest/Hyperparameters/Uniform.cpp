#include "Uniform.h"
#include <Utils.h>
#include <RandomNumberGenerator.h>
#include <cmath>

using namespace std;
using namespace DNest;

const double Uniform::logMinMu = log(1E-3);
const double Uniform::logMaxMu = log(1E3);

void Uniform::fromPrior()
{
	onFraction = randomU();
	mu = exp(logMinMu + (logMaxMu - logMinMu)*randomU());
}

double Uniform::perturb()
{
	int which = randInt(2);

	if(which == 0)
	{
		mu = log(mu);
		mu += pow(10., 1.5 - 6.*randomU())*randn();
		mu = mod(mu - logMinMu, logMaxMu - logMinMu) + logMinMu;
		mu = exp(mu);
	}
	else
	{
		onFraction += pow(10., 1.5 - 6*randomU())*randn();
		onFraction = mod(onFraction, 1.);
	}

	return 0.;
}

void Uniform::transform(double u_x, double u_y, double u_f,
				double& x, double& y, double& f) const
{
	// Compute flux
	if(u_f < onFraction)
		f = 0.;
	else
	{
		double U = (u_f - onFraction)/(1. - onFraction); // U(0, 1)
		f = -mu*log(U);
	}

	// Compute position
	x = u_x;
	y = u_y;
}

void Uniform::print(ostream& out) const
{
	out<<onFraction<<' '<<mu;
}


