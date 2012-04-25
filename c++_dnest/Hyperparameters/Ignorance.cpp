#include "Ignorance.h"

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
	Star s(0.,0.,0.);
	return s;
}


// Evaluate the probability density
// for a star given the hyperparameters
double Ignorance::_logp(const Star& star) const
{
	return 0.;
}

