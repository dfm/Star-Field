#ifndef _Ignorance_
#define _Ignorance_

#include "../Hyperparameters.h"
#include <ostream>

/*
* Stars can be anywhere
*/
class Ignorance:public Hyperparameters
{
	private:
		// Look ma, no parameters!

		// Well, some...
		static const double minFlux, maxFlux;
		static const double logMinFlux, logMaxFlux;


	public:

		/* Implement undefined methods */
		void fromPrior();
		double perturb();
		double perturb(std::vector<Star>& stars); // Take stars along for the ride

		// Generate a star from the prior
		// given the hyperparameters
		Star generateStar() const;
		double perturbStar(Star& star) const;

		// Evaluate the probability density
		// for a star given the hyperparameters
		double _logp(const Star& star) const;

		friend std::ostream& operator << (std::ostream& out, const Ignorance& i);

};

#endif

