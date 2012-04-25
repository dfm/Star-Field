#include "Ignorance.h"
#include "RandomNumberGenerator.h"
#include "../Data.h"
#include <cmath>

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
	double x = Data::get_image().get_xMin() + Data::get_image().get_xRange()*randomU();
	double y = Data::get_image().get_yMin() + Data::get_image().get_yRange()*randomU();
	double flux = exp(logMinFlux + (logMaxFlux - logMinFlux)*randomU());
	return Star(x, y, flux);
}

// Evaluate the probability density
// for a star given the hyperparameters
double Ignorance::_logp(const Star& star) const
{
	double result = 0.;
	if(star.x < Data::get_image().get_xMin() || star.x > Data::get_image().get_xMax()
		|| star.y < Data::get_image().get_yMin() || star.y > Data::get_image().get_yMax())
		result = -1E300;
	return result;
}

