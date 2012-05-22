#ifndef _Uniform_
#define _Uniform_

#include "../Hyperparameters.h"
#include <ostream>

/*
* Stars can be anywhere, but their fluxes are tied together
*/
class Uniform:public Hyperparameters
{
	private:
		double minLogMu, maxLogMu, rangeLogMu;

		double mu; // Mean flux of stars

	public:
		Uniform();

		// Generate hyperparameters from the prior
		void fromPrior();

		// Metropolis-Hastings proposal - move stars
		double perturb1(std::vector<Star>& stars);

		// Metropolis-Hastings proposal - don't move stars
		double perturb2(const std::vector<Star>& stars);

		/* Print the hyperparameters */
		void print(std::ostream& out) const;

		// Generate a star
		Star generateStar() const;

		// Evaluate the density
		double logp(const Star& star) const;

};

#endif

