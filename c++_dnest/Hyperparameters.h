#ifndef _Hyperparameters_
#define _Hyperparameters_

/*
* An abstract base class for describing
* models for the spatial and flux distribution
* of the stars.
*/

#include <vector>
#include <iostream>
#include "Star.h"

class Hyperparameters
{
	protected:
		// Parameters go here when you inherit

	public:
		/* Stuff that derived classes must implement */

		// Generate hyperparameters from the prior
		virtual void fromPrior() = 0;

		// Metropolis-Hastings proposal with stars
		virtual double perturb(std::vector<Star>& stars) = 0;

		/* Print the hyperparameters */
		virtual void print(std::ostream& out) const = 0;

		// Generate a star
		virtual Star generateStar() const = 0;

};

#endif

