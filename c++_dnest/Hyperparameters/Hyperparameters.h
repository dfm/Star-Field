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


	public:

		/* Stuff that derived classes must implement */
		virtual void fromPrior() = 0;
		virtual double perturb() = 0;
		virtual	double perturb(std::vector<Star>& stars) = 0; // Take stars along for the ride


};

#endif

