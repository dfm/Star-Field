#ifndef _NYU_
#define _NYU_

#include "../Hyperparameters.h"
#include <ostream>

/*
* Stars can be anywhere
*/
class NYU:public Hyperparameters
{
	private:
		double xc, yc, sigma;	// Positional
		double mu;		// Flux

		static const double minMu, maxMu, minSigma, maxSigma;

	public:

		/* Implement undefined methods */
		void fromPrior();
		double perturb();
		double perturb(std::vector<Star>& stars); // Take stars along for the ride

		// Generate a star from the prior
		// given the hyperparameters
		Star generateStar() const;
		double perturbPosition(Star& star, double scale) const;
		double perturbFlux(Star& star, double scale) const;

		// Evaluate the probability density
		// for a star given the hyperparameters
		double _logp(const Star& star) const;

		friend std::ostream& operator << (std::ostream& out, const NYU& i);

};

#endif

