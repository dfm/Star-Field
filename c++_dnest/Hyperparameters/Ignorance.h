#ifndef _Ignorance_
#define _Ignorance_

#include "../Hyperparameters.h"

/*
* Stars can be anywhere
*/
class Ignorance:public Hyperparameters
{
	private:
		// Look ma, no parameters!


	public:

		/* Implement undefined methods */
		void fromPrior();
		double perturb();
		double perturb(std::vector<Star>& stars); // Take stars along for the ride

		// Generate a star from the prior
		// given the hyperparameters
		Star generateStar() const;

		// Evaluate the probability density
		// for a star given the hyperparameters
		double _logp(const Star& star) const;

};

#endif

