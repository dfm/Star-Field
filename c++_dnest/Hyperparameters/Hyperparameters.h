#ifndef _Hyperparameters_
#define _Hyperparameters_

/*
* An abstract base class for describing
* models for the spatial and flux distribution
* of the stars.
*/

#include <vector>
#include "../Star.h"

class Hyperparameters
{
	protected:
		// Parameters go here when you inherit

	public:

		/* Stuff that derived classes must implement */
		virtual void fromPrior() = 0;
		virtual double perturb() = 0;
		virtual	double perturb(std::vector<Star>& stars) = 0; // Take stars along for the ride

		// Generate a star from the prior
		// given the hyperparameters
		virtual Star generateStar() const = 0;

		// Evaluate the probability density
		// for a star given the hyperparameters
		virtual double _logp(const Star& star) const = 0;
		double logp(const std::vector<Star>& stars) const;

};

#endif

