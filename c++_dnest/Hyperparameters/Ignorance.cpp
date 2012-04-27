#include "Ignorance.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include "../Data.h"
#include <cmath>

using namespace std;
using namespace DNest;

const double Ignorance::minFlux = 1E-3;
const double Ignorance::maxFlux = 1E3;
const double Ignorance::logMinFlux = log(Ignorance::minFlux);
const double Ignorance::logMaxFlux = log(Ignorance::maxFlux);

void Ignorance::fromPrior()
{

}

double Ignorance::perturb()
{
	return 0.;
}

double Ignorance::perturb(std::vector<Star>& stars)
{
	return 0.;
}

// Generate a star from the prior
// given the hyperparameters
Star Ignorance::generateStar() const
{
	double x = Data::get_data().get_xMin() + Data::get_data().get_xRange()*randomU();
	double y = Data::get_data().get_yMin() + Data::get_data().get_yRange()*randomU();
	double flux = exp(logMinFlux + (logMaxFlux - logMinFlux)*randomU());
	return Star(x, y, flux);
}

double Ignorance::perturbStar(Star& star) const
{
	double scale = pow(10., 1.5 - 6.*randomU());
	if(randomU() <= 0.5)
	{
		star.x += Data::get_data().get_xRange()*scale*randn();
		star.y += Data::get_data().get_yRange()*scale*randn();
		star.x = mod(star.x - Data::get_data().get_xMin(),
			Data::get_data().get_xRange()) + Data::get_data().get_xMin();
		star.y = mod(star.y - Data::get_data().get_yMin(),
			Data::get_data().get_yRange()) + Data::get_data().get_yMin();
	}
	else
	{
		star.flux = log(star.flux);
		star.flux += (logMaxFlux - logMinFlux)*scale*randn();
		star.flux = mod(star.flux - logMinFlux, logMaxFlux - logMinFlux) + logMinFlux;
		star.flux = exp(star.flux);
	}
	return 0.;
}

// Evaluate the probability density
// for a star given the hyperparameters
double Ignorance::_logp(const Star& star) const
{
	double result = 0.;
	if(star.x < Data::get_data().get_xMin() || star.x > Data::get_data().get_xMax()
		|| star.y < Data::get_data().get_yMin() || star.y > Data::get_data().get_yMax())
		result = -1E300;
	return result;
}

ostream& operator << (ostream& out, const Ignorance& i)
{
	return out;
}

