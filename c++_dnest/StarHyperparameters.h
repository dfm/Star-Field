#ifndef _StarHyperparameters_
#define _StarHyperparameters_

#include "Star.h"
#include <vector>
#include <ostream>

class StarHyperparameters
{
	friend class StarFieldModel;

	private:
		// Positional hyperparameters
		double xMean, yMean, sig;

		// Flux hyperparameters
		double meanFlux;

		static const double minMeanFlux;
		static const double maxMeanFlux;

	public:
		StarHyperparameters();			

		void fromPrior();
		double perturb();
		double perturb(std::vector<Star>& stars); // Take stars along for the ride

		// Generate a star from the prior
		// given the hyperparameters
		Star generateStar() const;

		// Evaluate the probability density
		// for a star given the hyperparameters
		double logp(const Star& star) const;
		double logp(const std::vector<Star>& stars) const;
		friend std::ostream& operator << (std::ostream& out, const StarHyperparameters& hyp);

};

#endif

